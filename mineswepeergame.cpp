#include<iostream>
#include<utility>
#include<vector>
using namespace std;


const int beginner=1;
const int intermediate=2;
const int advanced=3;
const int maxside=35;


int nrows;
int ncolumns;
int Mines;


void defficultylevel(){

    int selectlevel;

    cout<<"Enter the difficulty level"<<endl;
    cout<<"Select 1 for beginner => "<<endl;
    cout<<"Select 2 for Intermdiate => "<<endl;
    cout<<"Select 3 for advanced => "<<endl;

    cin>>selectlevel;

    if(selectlevel==beginner){
        nrows=9;
        ncolumns=9;
        Mines=10;
    }
    else if(selectlevel==intermediate){
        nrows=15;
        ncolumns=15;
        Mines=40;
    }else if(selectlevel==advanced){
        nrows=16;
        ncolumns=30;
        Mines=99;
    }

    return ;

}


void clearboard(char mineboard[][maxside],char gameboard[][maxside])
{
    for(int i=0;i<nrows;i++){
        for(int j=0;j<ncolumns;j++){
            mineboard[i][j]=gameboard[i][j]='.';
        }
    }
    return ;
}


void placemines(char mineboard[][maxside],int mines){
    int place=0;
    while(place<mines){
        int random=rand()%(nrows*ncolumns);
        int row=random/ncolumns;
        int col=random%nrows;
        if(mineboard[row][col]=='#'){continue;}
        mineboard[row][col]='#';
        place++;
    }
    return ;

}


void replacemines(int row,int col,char mineboard[][maxside]){
    placemines(mineboard,1);
    mineboard[row][col]='.';
    return ;

}


char indextochar(int index){
    if(index<10){return index+'0';}
    else{
        return 'a'+(index-10);
    }
}


int chartoindex(char ch){
    if(ch<='9'){return ch-'0';}
    else{return (ch-'a'+10);}
}


bool ismine(int row,int col,char board[][maxside]){
    return board[row][col]=='#';
}


bool isvalid(int row,int col){
    return (row>=0 )&& (row<nrows) &&(col>=0) && (col<ncolumns);
}


void displayboard(char gameboard[][maxside]){

    cout<<"  ";
    for(int i=0;i<ncolumns;i++){
        cout<<indextochar(i)<<" ";
    }
    cout<<endl<<endl;


    for(int i=0;i<nrows;i++){
        cout<<indextochar(i)<<"  ";
        for(int j=0;j<ncolumns;j++){
            cout<<gameboard[i][j]<<" ";
        }
        cout<<"  "<<indextochar(i);
        cout<<endl;
    }

    cout<<"  ";
    for(int i=0;i<nrows;i++){
        cout<<indextochar(i)<<" ";
    }
    cout<<endl<<endl;

    return ;
    
}


vector<pair<int,int>>getnehboiur(int row,int col){
    vector<pair<int,int>>neighbours;
    for(int dx=-1;dx<=1;dx++){
        for(int dy=-1;dy<=1;dy++){
            if(dx!=0 || dy!=0)
             if(isvalid(row+dx,col+dy)){
                 neighbours.push_back(make_pair(row+dx,col+dy));
             }
        }
    }

    return neighbours;
}


int countadjacentmines(int row,int col,char mineboard[][maxside]){
    int count=0;
      vector<pair<int,int>>neihbours=getnehboiur(row,col);
      
      for(int i=0;i<neihbours.size();i++){
          if(ismine(neihbours[i].first,neihbours[i].second,mineboard)){
              count++;
          }
      }
    
    return count;

}


void uncoverboard(char gameboard[][maxside],char mineboard[][maxside],int row,int col,int* nmoves){
    *nmoves++;
    int count=countadjacentmines(row,col,mineboard);

    gameboard[row][col]=count+'0';
    if(count==0){
        vector<pair<int,int>>neighbours=getnehboiur(row,col);
        for(int i=0;i<neighbours.size();i++){
            if(gameboard[neighbours[i].first][neighbours[i].second]=='.'){
                uncoverboard(gameboard,mineboard,neighbours[i].first,neighbours[i].second,nmoves);

            }
        }
    }

    return ;
}


void markmines(char gameboard[][maxside],char mineboard[][maxside],bool won){
    for(int i=0;i<nrows;i++){
        for(int j=0;j<ncolumns;j++){
            if(gameboard[i][j]=='.'&&gameboard[i][j]=='#'){
                if(won){
                    gameboard[i][j]='f';
                }
                else{
                    gameboard[i][j]='#';
                }
            }
        }

    }

    return ;
}


void playminesweeper(){
    char mineboard[maxside][maxside],gameboard[maxside][maxside];
    int movestotal=nrows*ncolumns-Mines;
    int flags=Mines;
    clearboard(mineboard,gameboard);
    placemines(mineboard,Mines);
    int nmoves=0;
    bool gameover=false;
    while(!gameover){
        displayboard(gameboard);
        cout<<flags<<"flags left"<<endl<<endl;

        char x,y,z;
        cout<<"enter your move for row,column,safe/flag: ";
        cin>>x>>y>>z;
        cout<<endl;

        int row=chartoindex(x);
        int col=chartoindex(y);
        if(nmoves==0){
            if(ismine(row,col,mineboard));
               {replacemines(row,col,mineboard);}   
        }

        if(z=='s'){
            if(gameboard[row][col]=='.' && mineboard[row][col]=='.'){
                uncoverboard(gameboard,mineboard,row,col,&nmoves);
                if(nmoves==movestotal){
                    markmines(gameboard,mineboard,true);
                    displayboard(gameboard);
                    cout<<endl<<"you won the game! "<<endl;
                    gameover=true;
                }
            }
        }
        else if (gameboard[row][col]=='.'&& mineboard[row][col]=='#'){
            gameboard[row][col]='#';
            markmines(gameboard,mineboard,false);
            displayboard(gameboard);
            cout<<endl<<"sorry you lose!"<<endl;
            gameover=true;
        }
        else{
            cout<<"illegal move";
            if(gameboard[row][col]=='F'){
                cout<<"cell is flag use f to toggle the flag.";}
                else{
                cout<<"cell is already a number.";

                }
            cout<<endl;
        }
       if(z=='f'){
           if(gameboard[row][col]=='.'){
               if(flags!=0){
                   gameboard[row][col]='F';
                   flags--;
               }
               else{
                   cout<<"illegal move too many flags."<<endl;

               }
           }
           else if(gameboard[row][col]=='F'){
               gameboard[row][col]='.';
               flags++;
           }
           else{
               cout<<"illegal move cell is number."<<endl;
           }
         }
    }

return ;

}


int main(){
    // srand(time(NULL));
    defficultylevel();
    playminesweeper();
    return 0;

}