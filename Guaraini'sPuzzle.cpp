#include <iostream>
#include <vector>

/*
* DEFINE CONSTANTS
*/
#define EMPTY 0
#define WHITE_KNIGHT 1 
#define BLACK_KNIGHT 2 
#define ROWS 4
#define COLUMNS 3
#define TOP_LEFT 1
#define TOP_CENTRE 2
#define TOP_RIGHT 3
#define BOTTOM_LEFT 10
#define BOTTOM_CENTRE 11
#define BOTTOM_RIGHT 12
#define NO_PIECES 12


class Board
{
    private:

        /*
        * COUNTER TO HOLD THE NUMBER OF MOVES DONE ON THE BOARD
        */
        int counter;

        /*
        * 2D MATRIX TO HOLD THE BOARD TILES
        */
        std::vector<std::vector<int>> tiles;


        /*
        * A VECTOR TO HOLD IF EACH TILE IS OCCUPIED OR NOT
        */
        std::vector<bool> occupied;

        /*
        * 2D MATRIX TO HOLD THE POSSIBLE MOVES
        */
        std::vector<std::vector<int>> graph;


    public:
        /*
        * OVERLOADING THE << OPERATOR TO PRINT THE BOARD
        */
        friend std::ostream& operator<<(std::ostream& os, const Board& board);

        /*
        * INTIALIZE THE BOARD WITH THE INTIAL SETTING
        */
        Board()
        {
            counter=0;

            tiles=std::vector<std::vector<int>>{
            {BLACK_KNIGHT,BLACK_KNIGHT,BLACK_KNIGHT}
            ,{EMPTY,EMPTY,EMPTY}
            ,{EMPTY,EMPTY,EMPTY}
            ,{WHITE_KNIGHT,WHITE_KNIGHT,WHITE_KNIGHT}
            };

            graph=std::vector<std::vector<int>>{
                {6,8},
                {7,9},
                {4,8},
                {3,9,11},
                {10,12},
                {1,7,11},
                {2,6,12},
                {1,3},
                {2,4,10},
                {5,9},
                {4,6},
                {5,7}
            };
        
            occupied=std::vector<bool>{
                true,true,true,
                false,false,false,
                false,false,false,
                true,true,true,
            };
        }

        /*
        * CONVERT FROM THE NUMBERING SYSTEM IN THE IMAGE INTO 2D INDICES
        */
        std::vector<int> convertTo2dIndex(int i)
        {
            int n=i-1;
            return std::vector<int>{n/COLUMNS,(n%COLUMNS)};
        }

        /*
        * CHECK IF THE GIVEN TILE IS EMPTY
        */
        bool isThatTileOccupied(int i)
        {
            auto indices = convertTo2dIndex(i);
            if(tiles[indices[0]][indices[1]]!=EMPTY)
                return true ;
            return false;
        } 

        bool isThatTileWhite(int i)
        {
            auto indices = convertTo2dIndex(i);
            if(tiles[indices[0]][indices[1]]==WHITE_KNIGHT)
                return true ;
            return false;
        }
        
        bool isThatTileBlack(int i)
        {
            auto indices = convertTo2dIndex(i);
            if(tiles[indices[0]][indices[1]]==BLACK_KNIGHT)
                return true ;
            return false;
        }

        /*
        * CHECKS IF THE BOARD HAS REACHED THE FINAL RESULT
        */
        bool hasReachedFinal()
        {
            for(int j=0;j<COLUMNS;j++)
            {
                if(tiles[0][j]!=WHITE_KNIGHT || tiles[3][j]!=BLACK_KNIGHT)
                    return false;
            }
            return true;
        }

        /*
        * CHECKS IF THE BOARD HAS REACHED STALEMATE (SPECIAL CASE 2)
        */
       bool hasReachedStaleMate()
       {
           std::vector<std::vector<int>> stalemate{
            {WHITE_KNIGHT,EMPTY,WHITE_KNIGHT}
            ,{EMPTY,EMPTY,EMPTY}
            ,{BLACK_KNIGHT,EMPTY,EMPTY}
            ,{BLACK_KNIGHT,BLACK_KNIGHT,WHITE_KNIGHT}
            };

            for(int i=0 ;i<ROWS;i++)
            {
                for(int j=0;j<COLUMNS;j++)
                {
                    if(stalemate[i][j]!=tiles[i][j])
                        return false;
                }
            }
            return true;
       }


        int getBottomLeftCorner()
        {
            return tiles[ROWS-1][0];
        }

        int getTopLeftCorner()
        {
            return tiles[0][0];
        }

        int getBottomRightCorner()
        {
            return tiles[ROWS-1][COLUMNS-1];
        }

        int getTopRight()
        {
            return tiles[0][COLUMNS-1];
        }

        int getTopCentre()
        {
            return tiles[0][COLUMNS/2];
        }

        int getBottomCentre()
        {
            return tiles[ROWS-1][COLUMNS/2];
        }

        /*
        * MAKE ONE MOVE ON THE CHESS BOARD
        */
        void makeMove(int start , int end )
        {
            counter++;
            auto startIndices=convertTo2dIndex(start);
            auto endIndices=convertTo2dIndex(end);
            std::swap(tiles[startIndices[0]][startIndices[1]],tiles[endIndices[0]][endIndices[1]]);
            occupied[start-1]=false;occupied[end-1]=true;
            std::cout<<"MOVE : " << counter <<std::endl<<*this<<std::endl<<std::endl;
        }

        /*
        * MAKE A NUMBER OF MOVES ON THE CHESS BOARD
        */
        void movePath(int start , std::vector<int> edges)
        {
            std::vector<std::vector<int>> path;
            for(int i=0;i<edges.size();i++)
            {
                path.push_back(std::vector<int>{start,edges[i]});
                start=edges[i];
            }

            for(int i=0;i<path.size();i++)
                makeMove(path[i][0],path[i][1]);
        }

        /*
        * GET THE PATH BETWEEN THE TWO GIVEN TILES
        */
        std::vector<int> getPath(int start , int end)
        {   bool finished = false;
            std::vector<bool>occupied=Board::occupied;
            std::vector<int>arr{};
            bool visited[NO_PIECES]={false};
            return getPathDFS(graph[start-1],visited,start,end,arr,occupied,&finished);
        }

        /*
        * RECURSIVE DEPTH-FIRST-SEARCH FUCNTION TO RETURN THE PATH
        */
        std::vector<int> getPathDFS(std::vector<int> possibleMoves,bool* visited ,int start 
        , int end,std::vector<int>path,std::vector<bool>occupied,bool* finished)
        {   
            std::vector<int> copied;
            if(start==end)
            {
                *finished=true;
                return path;
            }
            visited[start-1]=true;
            for(int i=0;i<possibleMoves.size();i++)
            {
                int element = possibleMoves[i];
                if(visited[element-1] || occupied[element-1])
                    continue;
                occupied[start-1]=false; occupied[element-1]=true;
                path.push_back(element);
                copied=getPathDFS(graph[element-1],visited,element,end,path,occupied,finished);
                if(!(*finished))
                    path.pop_back();
                else
                    return copied;
            }
            return copied;
        }

        /*
        * FUNCTION TO SOLVE GUARINI'S EXTEND PROBLEM
        */
        void solveGuarini()
        {
            /*
            * PRINTING THE BOARD ON START
            */
            std::cout << *this ; 

            /*
            * SPECIAL CASE 1 : MAKE FIRST TWO MOVES '1--->6','6--->7'
            */
            makeMove(1,6);
            makeMove(6,7);

            while(!hasReachedFinal())
            {
                int BOTTOM_CORNERS[]={BOTTOM_LEFT,BOTTOM_RIGHT};
                int TOP_CORNERS[]={TOP_LEFT,TOP_RIGHT};
                int OPTIONS_TOP_LEFT[]={BOTTOM_RIGHT,BOTTOM_CENTRE};
                int OPTIONS_TOP_RIGHT[]={BOTTOM_LEFT,BOTTOM_CENTRE};
                int OPTIONS_BOTTOM_LEFT[]={TOP_RIGHT,TOP_CENTRE};
                int OPTIONS_BOTTOM_RIGHT[]={TOP_LEFT,TOP_CENTRE};
                std::vector<int> path{};
                int start;

                /*
                * SPECIAL CASE 2 : SOLVE STALEMATE '7--->6'
                */
                if(hasReachedStaleMate())
                {
                    makeMove(7,6);continue;
                }

                /*
                * SPECIAL CASE 3 : ENDGAME '6--->7' , '7---12'
                */
                else if(counter==14)
                {
                    makeMove(6,7);makeMove(7,12);
                    continue;
                }


                /*
                  GENERAL CASE 1 : IF THERE'S A TILE IN ONE SIDE'S CENTRE EMPTY THEN
                                   ONE OF THE RIGHT OR LEFT OF THE OTHER SIDE SHOULD GO 
                                   THERE! 
                */
                else if(!isThatTileOccupied(TOP_CENTRE))
               {
                   for(int i=0;i<2;i++)
                   {
                       if(isThatTileWhite(BOTTOM_CORNERS[i]))
                       {
                           path = getPath(BOTTOM_CORNERS[i],TOP_CENTRE);
                           start= BOTTOM_CORNERS[i];
                           if(path.size()>0)
                                break;
                       }
                   }                    
               }

                else if(!isThatTileOccupied(BOTTOM_CENTRE))
                {
                    for(int i=0;i<2;i++)
                   {
                       if(isThatTileBlack(TOP_CORNERS[i]))
                       {
                           path = getPath(TOP_CORNERS[i],BOTTOM_CENTRE);
                           start = TOP_CORNERS[i];
                           if(path.size()>0)
                                break;
                       }
                   }   
                }

                /*
                  GENERAL CASE 2 : IF THERE'S A TILE IN ONE SIDE'S EDGE EMPTY THEN
                                   ONE OF THE CORRESPONDING OR CENTRE OF THE OTHER SIDE SHOULD GO 
                                   THERE! 
                */

               else if(!isThatTileOccupied(TOP_LEFT))
               {
                    for(int i=0;i<2;i++)
                   {
                       if(isThatTileWhite(OPTIONS_TOP_LEFT[i]))
                       {
                           path = getPath(OPTIONS_TOP_LEFT[i],TOP_LEFT);
                           start=OPTIONS_TOP_LEFT[i];
                           if(path.size()>0)
                                break;
                       }
                   }   
               }
                               
                else if(!isThatTileOccupied(TOP_RIGHT))
               {
                    for(int i=0;i<2;i++)
                   {
                       if(isThatTileWhite(OPTIONS_TOP_RIGHT[i]))
                       {
                           path = getPath(OPTIONS_TOP_RIGHT[i],TOP_RIGHT);
                           start=OPTIONS_TOP_RIGHT[i];
                           if(path.size()>0)
                                break;
                       }
                   }   
               }
            
                else if(!isThatTileOccupied(BOTTOM_LEFT))
               {
                    for(int i=0;i<2;i++)
                   {
                       if(isThatTileBlack(OPTIONS_BOTTOM_LEFT[i]))
                       {
                           path = getPath(OPTIONS_BOTTOM_LEFT[i],BOTTOM_LEFT);
                           start=OPTIONS_BOTTOM_LEFT[i];
                           if(path.size()>0)
                                break;
                       }
                   }   
               }

                else if(!isThatTileOccupied(BOTTOM_RIGHT))
               {
                    for(int i=0;i<2;i++)
                   {
                       if(isThatTileBlack(OPTIONS_BOTTOM_RIGHT[i]))
                       {
                           path = getPath(OPTIONS_BOTTOM_RIGHT[i],BOTTOM_RIGHT);
                           start=OPTIONS_BOTTOM_RIGHT[i];
                           if(path.size()>0)
                                break;
                       }
                   }   
               }
                
                movePath(start,path);

            }
        }


};

std::ostream& operator<<(std::ostream& os, const Board& board)
{
    os<<"_______"<<std::endl;
    for(int i=0; i<ROWS;i++)
    {
        for(int j=0;j<COLUMNS;j++)
            os<< '|'<<board.tiles[i][j] << "| ";
        os << std::endl << std::endl;
    }
    os<<"_______"<<std::endl;
    return os;
}


int main()
{
    Board board;
    board.solveGuarini();
}