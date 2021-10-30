#include <stdio.h>
#include <stdlib.h>     // NOTE : Bu oyunda system("cls") ve printf("\033[H\033[J") kullanilmistir,
#include <time.h>       //        Bunlar Cross-Platform olmadiklari icin bazi sistemlerde calismayacaktir
                        //        Line 188 ve 350 da olanlari sisteminize uygun bir sekilde degistirebilirsiniz.
int tetrominos[7][4][4]={    //   Tetris Parcalari
    {                        //   3-Boyutlu dizi olarak hard-code edilmis
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 }
    },
    {
        { 0, 0, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 1, 0 }
    }
};
int block[4][4]={0};    //tetris prcalarini random olarak alabilmem icin file-scope'unde olan 2 boyutlu dizi
int Board[21][12]={0}; //tetris ekrani, su an hepsi sifirdir.
void check_line();    //tetris ekranindaki ilk satiri surekli kontrol edip oyunun bitmesine yardimci olan fonksiyon
void Tetris_Delete_Line();//bir satir doldugunda, o satiri sifirlayan ve tetris ekranindaki parcalari asagiya kaydiran fonksiyon, scoru da gunceller
int Tetris_Board();      //  ilk basta tetris ekranini duzenleyen fonksiyon
int Show_Board(int Score);   // tetris ekraninin surekli ciktisini ekrana yazdiran ve ayni zamanda Score gosteren fonksiyon
int Add_Tetromino_to_Board();   // random bir parcayi alarak tatris ekranina ekleyen fonksiyon
int Random_Tetromino();        // random bir tetris parcasi olusturan fonksiyon
void Take_Input();            // kullanicidan input alan fonksiyon, mesela "d" harfi tetris parcalarini saga kaydirir.
int Move_Tetromino(int a,int b); // tetris parcalarini kullanicinin istegine gore kaydiran fonksiyon
int Tetromino_Rotation();        //  tetris parcalarini ceviren fonksiyon
int Check_Collision(int row,int column);// tetris parcalarinin bir yone kaydirilabilirligini belirten fonksiyon, rotate ederken da ise yarar
int Stop_Game();       // oyun bittiginda menu sayfasi
void Start_Game();     // oyunun main menu sayfasi
int On=1;              // while icinde ise yarar, check_line fonksiyonu degerini sifirladiginda oyun biter
int row_coordinate=0;
int column_coordinate=4;// cok ise yarayan degiskenler, oyun parcalarinin tam ortadan gelmelerini saglar, neredeyse her fonksiyonun icinde var.
int temporary_board[21][12]; // oyunun gizli kahramani, tetris ekraninin eski halini saklar, rotatelerde, kaydirmalarda cok ise yarayan bir dizi
int Score=0;       // puanlari saklar
int Press_Start(); // main menuda iken kullanicidan oyunun baslamasi icin inpu isteyen fonksiyon

int main()
{
    Start_Game();
    if(Press_Start())   // kullanici 1 degerini bastiginda 1, baska bir degere bastiginda 0 dondorur boylece program if'e girmis oluyor
    {
        Tetris_Board();      // tetris ekranini tasarlar
        Random_Tetromino(); //  random bir parca uretip ayni anda da add_tetromino_to_board fonksiyonunu kullanarak parcalari tetris ekranina ekler
        while(On)     // On degeri su an 1
        {
            Show_Board(Score);   // tetris ekranini ve scoru yazdirir

            if (Check_Collision(row_coordinate+1,column_coordinate))// tetris parcasinin asagiya gidebilme ihtimali varsa kullanicidan input alir
            {
                Take_Input();    // kullanicidan input alan fonksiyon, ayni zamanda input aldiktan sonra parcayi hemen kaydirir
            }
            else
            {
                Tetris_Delete_Line();
                for(int i=0;i<21;i++)
                {
                    for(int j=0;j<12;j++)
                    {
                        temporary_board[i][j]=Board[i][j];// temporary_board tetris ekraninin eski halini saklar, kontrol icin onemli
                    }
                }
                check_line();
                Random_Tetromino();
            }

        }
        Stop_Game();
    }



    return 0;
}
int Press_Start()
{
    int input;
    scanf("%d",&input);
    switch (input)
    {
        case 1: return 1;
        case 2: return 0;
        default:{ printf("Only Enter 1 or 2");
                  return 0;}
    }
}
void Tetris_Delete_Line()
{
    int tmp,count=0;
    for(int i=19;i>0;i--)
    {
        int count=0;
        for(int j=1;j<11;j++)  // tetris ekraninin satirinin hepsinin dolu olup olmadigini kontrol eder
        {
            if(Board[i][j]!=0)
            {
                count++;
            }
        }

        if(count==10)       //  count 10 ise dolu demek cunku 10 tane sutunumuz var
        {
            Score=Score+300; // herdolu olup sonra sifirlanan satir icin 300 puan yeter bence
            for(int j=1;j<11;j++)
            {
                Board[i][j]=0;
            }
            for(int k=i;k>0;k--)  // satir sifirlandi simdi matris ekranini asagiya kaydirma zamani
            {
                for(int j=1;j<11;j++)
                {
                    tmp=Board[k-1][j];
                    Board[k-1][j]=Board[k][j];
                    Board[k][j]=tmp;
                }
            }
            i++;

        }
    }
}
void check_line()
{
    int count=0;
    for(int j=1;j<11;j++)
    {
        if(Board[1][j]!=0)
        {
            count++;
        }
    }
    if(count!=0)
    {
        On=0;
    }
}
int Tetris_Board(int Score)    //  tetris ekranini tasarlar
{
    int i,j;
    for(i=0;i<21;i++)
    {
        for(j=0;j<12;j++)
        {
            if(j==0 || j==11 || i==20)  // tetris ekraninin kenarlarini sekillendiri
            {
                Board[i][j]=temporary_board[i][j]=10;
            }
            else
                Board[i][j]=temporary_board[i][j]=0;
        }
    }
    return 0;
}
int Show_Board(int Score)    // tetris ekranini yazdirir
{
    printf("\033[H\033[J");   // system("cls")'tan daha iyidir diye duydum
    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            switch (Board[i][j])
            {
            case 0:
                printf(" ");  // tetris matrisinin elemani sifir ise bos kalsin
                break;
            case 10:
                printf("%c",219); // matrisin kenarlari
                break;
            default:
                printf("@",254);  // sifir ve ondan farkli herhangi bir sayi icin # yazdirsin
                break;
            }
        }
        printf("\n");
    }

    printf("\n\tA: left\tS: down\tD: right \t Rotate: Space      Score: %d",Score);
    printf("\n");
    return 0;
}
int Random_Tetromino() // random parca uretir
{
    row_coordinate=0;column_coordinate=4;// kaydirmalardan sonra degerleri degistigi icin yeni parcalar olusturuldugunda yine eski degerlerine donsun
    int i,j;
    int number;
    srand(time(0));
    number=rand()%7; // 0 ve 6 araliginda random sayi uretir
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            block[i][j]=tetrominos[number][i][j]; // 3-boyutlu parcalar dizimizi kullandik
        }
    }
    Add_Tetromino_to_Board();  // parcayi tetris matrisine ekler
    return 0;
}
int Add_Tetromino_to_Board()
{
    int i,j;
    for ( i = 0; i < 4; i++)
    {
        for ( j = 0; j < 4; j++)
        {
            Board[i][j + 4] = block[i][j];
        }
    }
    return 0;
}
int Move_Tetromino(int a,int b) // parcalari kaydirir, kullanicinin inputuna gore take_input fonksiyonun icinde a ve b parametreleri belirlenir
{
    int i,j;

    for ( i = 0; i < 4; i++)
    {
        for ( j = 0; j < 4; j++)
        {
            Board[row_coordinate + i][column_coordinate + j] -= block[i][j];// prcayi eski yerinden siler
        }
    }
    row_coordinate=a;
    column_coordinate=b;
    for ( i = 0; i < 4; i++)
    {
        for ( j = 0; j < 4; j++)
        {
            Board[row_coordinate + i][column_coordinate + j] += block[i][j]; // a ve b parametrelerine gore parcayi baska yere yazdirir
        }
    }
    return 0;
}
void Take_Input()
{
    char input;
    input= getch();

    switch (input)
    {
        case 'd': if(Check_Collision(row_coordinate,column_coordinate+1))// baska bir parca ile carpisma yoksa 1 dondorur aksi halde 0
                  {
                    Move_Tetromino(row_coordinate,column_coordinate+1);
                  }
                    break;
        case 's': if(Check_Collision(row_coordinate+1,column_coordinate))
                  {
                    Move_Tetromino(row_coordinate+1,column_coordinate);
                  }
                    break;
        case 'a': if(Check_Collision(row_coordinate,column_coordinate-1))
                  {
                    Move_Tetromino(row_coordinate,column_coordinate-1);
                  }
                    break;
        case ' ': {Tetromino_Rotation();}
                  break;
    }

}
int Tetromino_Rotation()
{
    int tmp[4][4]={0};  // parcalari dondormadan once bir kopyalarini alsam daha iyi olur diye dusundum

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tmp[i][j] = block[i][j];
        }
    }

    for (int i = 0; i < 4; i++)    // pracayi rotateler
    {
        for (int j = 0; j < 4; j++)
        {
            block[i][j] = tmp[3 - j][i];
        }
    }

    if (!Check_Collision(row_coordinate,column_coordinate)) // baska parca ile carpisiyorsa, parca eski haline donsun
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                block[i][j] = tmp[i][j];
            }
        }
        return 0;
    }

    for (int i = 0; i < 4; i++)   //  parcayi tetris ekranina ekler, cevirilmis hali ise guzel , degilse de sikinti yok
    {
        for (int j = 0; j < 4; j++)
        {
            Board[row_coordinate + i][column_coordinate + j] -= tmp[i][j];
            Board[row_coordinate + i][column_coordinate + j] += block[i][j];
        }
    }
    return 1;
}
int Check_Collision(int row,int column)
{
    int i,j;
    for ( i = 0; i < 4; i++)
    {
        for ( j = 0; j < 4; j++)
        {
            if (block[i][j] && temporary_board[row + i][column + j] != 0)// iste burada tetris ekraninin bir kopyasi cok ise yaradi
            {                                    // parca ve matrisin kopyasinin tekabul eden elemanlari 0 degilse carpisma var demek
                return 0;
            }
        }
    }
    return 1;
}
void Start_Game()
{
    system("cls");

    printf("#==============================================================================#\n");

    printf("####### ####### ####### ######    ###    #####\n");
    printf("   #    #          #    #     #    #    #     #\n");// string concatenation daha iyi olurdu ama yine de fena degil.
    printf("   #    #          #    #     #    #    #\n");
    printf("   #    #####      #    ######     #     #####\n");
    printf("   #    #          #    #   #      #          #\n");
    printf("   #    #          #    #    #     #    #     #\n");
    printf("   #    #######    #    #     #   ###    #####\t");
    printf("\n\n\n\n");

    printf("\t<Menu>\n");
    printf("\t1: Start Game\n\t2: Quit\n\n");

    printf("#==============================================================================#\n");
    printf("Choose >> ");
}
int Stop_Game()
{
    char a;
    printf(" #####     #    #     # ####### ####### #     # ####### ######\n") ;
    printf("#     #   # #   ##   ## #       #     # #     # #       #     #\n");
    printf("#        #   #  # # # # #       #     # #     # #       #     #\n");
    printf("#  #### #     # #  #  # #####   #     # #     # #####   ######\n");
    printf("#     # ####### #     # #       #     #  #   #  #       #   #\n");
    printf("#     # #     # #     # #       #     #   # #   #       #    #\n");
    printf(" #####  #     # #     # ####### #######    #    ####### #     #\n");
    printf("\n\nPress any key and enter\n");
    scanf("%c",&a);
    return 0;
}
