//NessRidge Company Period: 5
import java.util.*;
public class BattleShip {
    public static void main (String [] args) {
        if (displayTitleScreen()) {
            Scanner pencil = new Scanner (System.in);
            boolean playerWin = false, compWin = false;
            ArrayList <ArrayList <Integer>> nextMove = new ArrayList <ArrayList <Integer>>();
            BattleShipBoard playerBoard = new BattleShipBoard();
            BattleShipBoard compBoard = new BattleShipBoard();
            placeShip(playerBoard.getShips(), playerBoard.getBoard());
            compPlaceShip(compBoard.getShips(), compBoard.getBoard());
            for (int turn = 0; !playerBoard.isWin() || !compBoard.isWin(); turn++) {
                if (!compWin) playerWin = mainGamePlayer(compBoard, playerBoard.getBoard(), turn, playerBoard);
                if (!playerWin) {
                    nextMove = mainGameComp (playerBoard, compBoard.getBoard(), turn, nextMove);
                    System.out.println(nextMove.size());
                    for (ArrayList <Integer> i : nextMove)
                        System.out.println(i.get (0) + ", " + i.get(1));
                    if (nextMove.size() != 0) compWin = true;
                    else compWin = false;
                }
                if (playerWin || compWin) turn--;
            }
            if(compBoard.isWin()) System.out.println("LADIES AND GENTLEMEN, WE GOT EM \n             YOU WIN");
            else if(playerBoard.isWin()) System.out.println("Mission failed, we'll get em next time... \n             Computer wins");
        }
    }
    public static void placeShip (Ship [] ships, String [] [] b) {
        boolean validMove1 = false, validMove2 = false;
        int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
        Scanner pencil = new Scanner (System.in);
        for (int i = 0; i < 5; i++) {
            while (!validMove1) {
                displayPlacingBoard(b);
                System.out.println("               Now placing: " + ships [i].getName() + "(" + ships [i].getToken() + "), Size: " + ships [i].getSize());
                x1 = pencil.nextInt() - 1;
                y1 = 8 - pencil.nextInt();
                if (x1 < 8 && x1 >= 0 && y1 >= 0 && y1 < 8) {
                    if (b [y1] [x1] == " ")
                        if (ableToPlacePiece(x1, y1, i, b).size() > 0 || i == 0) {
                            ships [i].getPieces() [0].setX(x1);
                            ships [i].getPieces() [0].setY(y1);
                            b [ships [i].getPieces() [0].getY()] [ships [i].getPieces() [0].getX()] = ships [i].getToken();
                            if (i > 0) displayPlacingBoard(b);
                            validMove1 = true;
                        } else System.out.println("Soldier, this isn't a board game. Ships don't just stack up on each other.");
                } else System.out.println("Soldier, we ain't trying to attack other countries lmao.");
            }
            while (!validMove2) {
                validMove2 = true;
                if (i != 0) {
                    x2 = pencil.nextInt() - 1;
                    y2 = 8 - pencil.nextInt();
                    if ((x2 < 8 && x2 >= 0 && y2 >= 0 && y2 < 8) && (Math.abs(x1 - x2) == i && y1 == y2) || (Math.abs(y1 - y2) == i && x1 == x2)) {
                        if (b [y2] [x2] == " ") {//Add Condition for enclosed Spaces here
                            for (int j = 1; j <= Math.abs((y1 - y2) + (x1 - x2)); j++)
                                if ((x1 != x2) && (b [y1] [x1 + ((x2 - x1) / (Math.abs(x2 - x1))) * (j)] != " ") || (y1 != y2) && b [(y1 + ((y2 - y1) / (Math.abs(y2 - y1))) * (j))] [x1] != " ")
                                    validMove2 = false;
                            for (int j = 1; j <= Math.abs((y1 - y2) + (x1 - x2)) && validMove2; j++)
                                if (x1 != x2) {
                                    ships [i].getPieces() [j].setX(x1 + ((x2 - x1) / (Math.abs(x2 - x1))) * (j));
                                    ships [i].getPieces() [j].setY(y1);
                                    b [ships [i].getPieces() [j].getY()] [ships [i].getPieces() [j].getX()] = ships [i].getToken();
                                } else {
                                    ships [i].getPieces() [j].setX(x1);
                                    ships [i].getPieces() [j].setY(y1 + ((y2 - y1) / (Math.abs(y2 - y1))) * (j));
                                    b [ships [i].getPieces() [j].getY()] [ships [i].getPieces() [j].getX()] = ships [i].getToken();
                                }
                        } else validMove2 = false;
                    } else validMove2 = false;
                }
                if (!validMove2) {
                    displayPlacingBoard(b);
                    System.out.println("               Now placing: " + ships [i].getName() + "(" + ships [i].getToken() + "), Size: " + ships [i].getSize());
                }
            }
            validMove1 = validMove2 = false;
        }
    }
    public static ArrayList <ArrayList <Integer>> ableToPlacePiece (int x1, int y1, int i, String [] [] b) {
        int count = 0;
        ArrayList <ArrayList <Integer>> possibleMove = new ArrayList <ArrayList <Integer>>();
        for (int k = 0; k < 8; k++)
            for (int m = 0; m < 8; m++)
                if (((Math.abs(y1 - k) == i) && (x1 == m)) || ((Math.abs(x1 - m) == i) && (y1 == k))) {
                    boolean pieceDetected = false;
                    for (int j = 1; j <= Math.abs((y1 - k) + (x1 - m)); j++)
                        if ((x1 != m) && (b [y1] [x1 + ((m - x1) / (Math.abs(m - x1))) * (j)] != " ") || (y1 != k) && b [(y1 + ((k - y1) / (Math.abs(k - y1))) * (j))] [x1] != " ")
                            pieceDetected = true;
                    if (!pieceDetected) {
                        possibleMove.add(new ArrayList <Integer>());
                        possibleMove.get(count).add(m);
                        possibleMove.get(count).add(k);
                        count++;
                    }
                }
        return possibleMove;
    }
    public static void compPlaceShip (Ship [] ships, String [] [] b) {
        boolean validMove1 = false, validMove2 = false;
        int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
        Scanner pencil = new Scanner (System.in);
        for (int i = 4; i >= 0; i--) {
            while (!validMove1) {
                x1 = (int) (Math.random() * 7);
                y1 = (int) (Math.random() * 7);
                if (b [y1] [x1] == " ")
                    if (ableToPlacePiece(x1, y1, i, b).size() > 0 || i == 0) {
                        ships [i].getPieces() [0].setX(x1);
                        ships [i].getPieces() [0].setY(y1);
                        //b [ships [i].getPieces() [0].getY()] [ships [i].getPieces() [0].getX()] = ships [i].getToken();
                        validMove1 = true;
                    }
            }
            while (!validMove2 && i > 0) {
                ArrayList <ArrayList <Integer>> possibleMove = ableToPlacePiece(x1, y1, i, b);
                validMove2 = true;
                x2 = possibleMove.get((int) (Math.random() * possibleMove.size())).get(0);
                y2 = possibleMove.get((int) (Math.random() * possibleMove.size())).get(1);
                if ((x2 < 8 && x2 >= 0 && y2 >= 0 && y2 < 8) && (Math.abs(x1 - x2) == i && y1 == y2) || (Math.abs(y1 - y2) == i && x1 == x2)) {
                    if (b [y2] [x2] == " ") {//Add Condition for enclosed Spaces here
                        for (int j = 1; j <= Math.abs((y1 - y2) + (x1 - x2)); j++)
                            if ((x1 != x2) && (b [y1] [x1 + ((x2 - x1) / (Math.abs(x2 - x1))) * (j)] != " ") || (y1 != y2) && b [(y1 + ((y2 - y1) / (Math.abs(y2 - y1))) * (j))] [x1] != " ")
                                validMove2 = false;
                        for (int j = 1; j <= Math.abs((y1 - y2) + (x1 - x2)) && validMove2; j++)
                            if (x1 != x2) {
                                ships [i].getPieces() [j].setX(x1 + ((x2 - x1) / (Math.abs(x2 - x1))) * (j));
                                ships [i].getPieces() [j].setY(y1);
                                //b [ships [i].getPieces() [j].getY()] [ships [i].getPieces() [j].getX()] = ships [i].getToken();
                            } else {
                                ships [i].getPieces() [j].setX(x1);
                                ships [i].getPieces() [j].setY(y1 + ((y2 - y1) / (Math.abs(y2 - y1))) * (j));
                                //b [ships [i].getPieces() [j].getY()] [ships [i].getPieces() [j].getX()] = ships [i].getToken();
                            }
                    } else validMove2 = false;
                } else validMove2 = false;
            }
            validMove1 = validMove2 = false;
        }
        displayPlacingBoard(b);
    }
    public static boolean mainGamePlayer (BattleShipBoard compBoard, String [] [] playerBoard, int turn, BattleShipBoard player) {
        //M for miss, H for hit, D for destroyed
        boolean validMove = false, sunk = false;
        int x1 = 0, y1 = 0;
        Scanner pencil = new Scanner (System.in);
        while (!validMove) {
            displayPlayingBoard(compBoard.getBoard(), playerBoard, player);
            System.out.println("Enter the Square to fire at: ");
            x1 = pencil.nextInt() - 1;
            y1 = 8 - pencil.nextInt();
            if (x1 < 8 && x1 >= 0 && y1 < 8 && y1 >= 0) {
                if (!compBoard.getBoard() [y1] [x1].equals("M") && !compBoard.getBoard() [y1] [x1].equals("X") && !compBoard.getBoard() [y1] [x1].equals("H")) {
                    validMove = true;
                    for (Ship ship: compBoard.getShips())
                        for (Piece piece: ship.getPieces()) {
                            if (x1 == piece.getX() && y1 == piece.getY()) {
                                compBoard.getBoard() [y1] [x1] = "H";
                                piece.setHit(true);
                                sunk = true;
                                if (ship.isSunk()) {
                                    ship.setSunk(true);
                                    for (Piece p: ship.getPieces())
                                        compBoard.getBoard() [p.getY()] [p.getX()] = "X";
                                }
                            } else if (!sunk) compBoard.getBoard() [y1] [x1] = "M";                    
                        }
                } else System.out.println("Soldier, stop hiting areas that have been hit already.");
            } else System.out.println("Soldier, where on Eeeearth are you aimin'?");
        }
        return sunk;
    }
    public static ArrayList <ArrayList <Integer>> mainGameComp (BattleShipBoard playerBoard, String [] [] compBoard, int turn, ArrayList <ArrayList <Integer>> nextMove) {
        //M for miss, H for hit, D for destroyed
        ArrayList <ArrayList <Integer>> a = new ArrayList <ArrayList <Integer>>();
        boolean validMove = false, sunk = false;
        int x1 = 0, y1 = 0;
        Scanner pencil = new Scanner (System.in);
        while (!validMove) {
            if (nextMove.size() == 0) {
                x1 = (int) (Math.random() * 7);
                y1 = (int) (Math.random() * 7);
            } else {
                x1 = nextMove.get((int) (Math.random() * nextMove.size())).get(0);
                y1 = nextMove.get((int) (Math.random() * nextMove.size())).get(1);
            }
            if (!playerBoard.getBoard() [y1] [x1].equals("M") && !playerBoard.getBoard() [y1] [x1].equals("X") && !playerBoard.getBoard() [y1] [x1].equals("H")) {
                validMove = true;
                for (Ship ship: playerBoard.getShips())
                    for (Piece piece: ship.getPieces()) {
                        if (x1 == piece.getX() && y1 == piece.getY()) {
                            playerBoard.getBoard() [y1] [x1] = "H";
                            piece.setHit(true);
                            sunk = true;
                            if (ship.isSunk()) {
                                ship.setSunk(true);
                                for (Piece p: ship.getPieces())
                                    playerBoard.getBoard() [p.getY()] [p.getX()] = "X";
                            }
                        } else if (!sunk) playerBoard.getBoard() [y1] [x1] = "M";                    
                    }
            }
        }
        //displayPlayingBoard(playerBoard.getBoard(), compBoard);
        if (sunk) return compAI (playerBoard, x1, y1);
        return a;
    }
    public static ArrayList <ArrayList <Integer>> compAI (BattleShipBoard playerBoard, int x1, int y1) {
        int count = 0;
        ArrayList <ArrayList <Integer>> possibleMove = new ArrayList <ArrayList <Integer>>();
        for (int k = 0; k < 8; k++)
            for (int m = 0; m < 8; m++)
                if (((Math.abs(y1 - k) == 1) && (x1 == m)) || ((Math.abs(x1 - m) == 1) && (y1 == k))) 
                    if (playerBoard.getBoard() [k] [m] != "M" && playerBoard.getBoard() [k] [m] != "H" && playerBoard.getBoard() [k] [m] != "X") {
                        possibleMove.add(new ArrayList <Integer>());
                        possibleMove.get(count).add(m);
                        possibleMove.get(count).add(k);
                        count++;
                    }
        return possibleMove;
    }
    public static void displayPlacingBoard (String [] [] b) {
        System.out.println();
        System.out.println("                            Soldier, enter the coordinate points of the ends of the ships when placing them.");
        for (int i = 0; i < 2; i++)
            System.out.println();
        System.out.println("                                                                 Home Base              ");
        System.out.print("                                                     ---------------------------------");
        for (int i = 0; i < 8; i++) {
            if (i > 0)
                System.out.print("\n                                                     | - + - + - + - + - + - + - + - |");
            System.out.print("\n                                                   " + (8 - i) + " | ");
            for (int j = 0; j < 8; j++) 
                System.out.print(b [i] [j] + " | ");
        }
        System.out.println("\n                                                     ---------------------------------\n                                                       1   2   3   4   5   6   7   8");
        for (int i = 0; i < 4; i++)
            System.out.println();
    }
    public static void displayPlayingBoard (String [] [] b1, String [] [] b2, BattleShipBoard player) {
        System.out.println();
        System.out.println();
        System.out.println();
        System.out.println("                                                                                                                                        ");
        System.out.println("                                  The Opps                                         Home Base                                                           ");
        System.out.print("                      ---------------------------------                ---------------------------------                                                 ");
        for (int i = 0; i < 8; i++) {
            if (i > 0)
                System.out.print("\n                      | - + - + - + - + - + - + - + - |                | - + - + - + - + - + - + - + - |                                       ");
            System.out.print("\n                    " + (8 - i) + " | ");
            for (int j = 0; j < 8; j++) 
                System.out.print(b1 [i] [j] + " | ");
            System.out.print("             " + (8 - i) + " | ");
            for (int j = 0; j < 8; j++) 
                System.out.print(b2 [i] [j] + " | ");
        }
        System.out.println();
        System.out.println("                      ---------------------------------                ---------------------------------                                               ");
        System.out.println("                        1   2   3   4   5   6   7   8                    1   2   3   4   5   6   7   8                                                 ");
        System.out.println();
        System.out.println();
        System.out.print("        " + player.getShips() [0]);
        System.out.print(player.getShips() [1]);
        System.out.print(player.getShips() [2]);
        System.out.print(player.getShips() [3]);
        System.out.print(player.getShips() [4]);
        System.out.println();
        System.out.println();
    }
    public static boolean displayTitleScreen () {
        Scanner pencil = new Scanner (System.in);
        System.out.println("                                                                                                                                    ");
        System.out.println("                                                                                                                                    ");
        System.out.println("                                                                                                      ===========================   ");
        System.out.println("                                                                                                      |      _____________      |   ");
        System.out.println("        =-----  -=====- ---=--- ---=--- |       =------ =------ |     | ---=--- |------               |      |  __   __  |      |   ");
        System.out.println("        |     | |     |    |       |    |       |       |       |     |    |    |     |               |      |  O     O  |      |   ");
        System.out.println("        +-----  |=====|    |       |    |       |------ +------ |=====|    |    |-----|               |      |    ___    |      |   ");
        System.out.println("        |     | |     |    |       |    |       |             | |     |    |    |                     |      |    ---    |      |   ");
        System.out.println("        =-----  |     |    |       |    |______ =------ ------| |     | ---=--- |                     |      |===========|      |   ");
        System.out.println("     _____________________________________________________________________________________            |     /|           |\\     |   ");
        System.out.println("                                                                                                      |=========================|   ");
        System.out.println("    +------------------------------------------------------------------------------------+            |                         |   ");
        System.out.println("    |                                                                                    |            |  Soldier! If you want   |   ");
        System.out.println("    |                        ^                                                           |            |  to go to war and earn  |   ");
        System.out.println("    |                       -|-                                                          |            |  glory, respond with    |   ");
        System.out.println("    |                        |                                                           |            |        YESSIR           |   ");
        System.out.println("    |                      __|__                                                         |            |  Anything else will     |   ");
        System.out.println("    |                        |                _                                          |            |  mean insubordination   |   ");
        System.out.println("    |                        |               |.|                                         |            |  and this call will be  |   ");
        System.out.println("    |                       /|              -[:]-          ___                           |            |  terminated!            |   ");
        System.out.println("    |                      /'|         __    |.|  ___     :  c >                         |            |                         |   ");
        System.out.println("    |                      |`|    ___  ||   |!.|  |  C >  |  |                           |            |                         |   ");
        System.out.println("    |                -=-==/! :==-=|*|-=||-=--  -==|--|=-=-'- '-=-==--->                  |            |                         |    ");
        System.out.println("    |                \\  o  <>  O  - -  '' <=>--<> '  '  o   O   o   _/                   |            |                         |   ");
        System.out.println("    |                 \\_ _  _  _  _  _ _ _  _  _ __  _  _  _  __  _/                     |            |                         |   ");
        System.out.println("    |                                                                                    |            |                         |   ");
        System.out.println("    |                                                                                    |            |                         |   ");
        System.out.println("    +------------------------------------------------------------------------------------+            ===========================                    ");
        String input = pencil.nextLine();
        if (input.equals("YESSIR"))
            return true;
        else return false;
    }
}