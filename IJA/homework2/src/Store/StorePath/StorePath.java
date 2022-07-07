package Store.StorePath;

import Store.StoreData.StoreMap;
import Store.StoreObject.*;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.PriorityQueue;
import java.util.Queue;

/**
 * Class that represents findingshortest path
 *
 *
 * @author xvenge01 Veronika Vengerova
 */
public final class StorePath {
    private final static int DiagonalCost = 14; //v jednom
    private final static int V_H_cost = 10;
    private static Cell[][] mapa;
    private static Queue<ArrayList<Integer>> queue = new LinkedList<>();
    private static int distanceofthispath = 0;
    private static PriorityQueue<Cell> openCells;
    private static boolean[][] closedCells;
    private static int startx, starty;
    private static int endx, endy;

    /**
     * Adds block to map representation of storemap.
     *
     * @param i x coordinates of block
     * @param j y coordinates of block
     */
    private static void addBlock(int i,int j){
        mapa[i][j] = null;
    }

    /**
     * Initializes and processes map needed for future processing and finding minimal path
     *
     * @param strmap the map we want to find the the shortest path in
     * @param sx x coordinates of starting point
     * @param sy y coordinates of starting point
     * @param ex x coordinates of ending point
     * @param ey y coordinates of ending point
     */
    private static void initForPath(StoreMap strmap, int sx, int sy, int ex, int ey){
        if(strmap==null)
            return;
        int width = strmap.getSizeX(); //sirka mapy
        int height = strmap.getSizeY();//vyska mapy

        mapa = new Cell[width][height];
        closedCells = new boolean[width][height];
        openCells = new PriorityQueue<>((Cell c1, Cell c2) -> {
            return c1.finalCost < c2.finalCost ? -1 : c1.finalCost > c2.finalCost ? 1 : 0;
        });

        endCell(ex, ey);
        StartCell(sx,sy);
        for (int i = 0; i < mapa.length; i++) {
            for (int j = 0; j < mapa[i].length; j++) {
                mapa[i][j] = new Cell(i, j);
                mapa[i][j].heuristicCost = Math.abs(i - endx) + Math.abs(j - endy);
            }
        }
        mapa[startx][starty].finalCost = 0;//startingpoint
        processMap(strmap);
    }


    /**
     * Gets the minimal path from 1. coordinates to 2. coordiantes
     * @param strmap
     * @param sx
     * @param sy
     * @param ex
     * @param ey
     * @return
     */
    public static Queue<ArrayList<Integer>> getPath(StoreMap strmap, int sx, int sy, int ex, int ey){
        queue.clear();
        initForPath(strmap,sx,sy,ex,ey);
        process();
        return getPathQueue();
    }


    /**
     * Processes map for finding shortest path, represents blocks as blocks.
     *
     * @param strmap the map we want to find the the shortest path in
     */
    private static void processMap(StoreMap strmap){
        if(strmap==null)
            return;
        StoreObject[] strobj = strmap.getMap();
        for(StoreObject obj : strobj){
            if (obj!=null) {
                if (obj.getType() == StoreObject.Type.Shelf ||
                        obj.getType() == StoreObject.Type.Blockade ||
                        obj.getType() == StoreObject.Type.PickupSpot
                ) {
                    if ((obj.getX() != startx || obj.getY() != starty) && (obj.getX() != endx || obj.getY() != endy)) {
                        addBlock(obj.getX(), obj.getY());
                    }
                }
            }
        }
    }

    /**
     * Gets the path in form of Queue
     *
     * @return queue with array of x,y coordinates of path stored in arrayList
     */
    private static Queue<ArrayList<Integer>> getPathQueue(){
        if(mapa == null)
            return null;
        if(closedCells[endx][endy]) {
            Cell current = mapa[endx][endy];
            ArrayList<Integer> newArrmin =new ArrayList<>();
            newArrmin.add(distanceofthispath);
            queue.add(newArrmin);
            while(current.parent != null){
                ArrayList<Integer> newArr1 =new ArrayList<>();
                newArr1.add(current.i);
                newArr1.add(current.j);
                queue.add(newArr1);
                current = current.parent;
            }
            //posledne sme prisli az na koniec, v zoznamev queue je ako posledne aj vzdialenost
            ArrayList<Integer> newArr =new ArrayList<>();
            newArr.add(current.i);
            newArr.add(current.j);
            queue.add(newArr);

        }
        return queue;
    }

    /**
     * Sets starting cell
     * @param i x coordinates of starting point
     * @param j y coordinates of starting point
     */
    private static void StartCell(int i, int j){
        startx = i;
        starty = j;
    }

    /**
     * Sets starting cell
     * @param i x coordinates of ending point
     * @param j y coordinates of ending point
     */
    private static void endCell(int i, int j){
        endx = i;
        endy = j;
    }

    /**
     *  Updates the theoretical values if the new path is shorter
     * @param current current cell
     * @param neighborcell neighboring cell
     * @param cost the cost/distance
     */
    private static void updateCostIfNeeded(Cell current, Cell neighborcell, int cost){
        if (neighborcell == null || closedCells[neighborcell.i][neighborcell.j]){
            return;
        }

        int neighborFinalcost = neighborcell.heuristicCost + cost;
        boolean isOpen = openCells.contains(neighborcell);

        if(!isOpen || neighborFinalcost < neighborcell.finalCost){
            neighborcell.finalCost = neighborFinalcost;
            neighborcell.parent = current;

            if(!isOpen){
                openCells.add(neighborcell);
            }
        }
    }

    /**
     * Gets the shortest distance of this path.
     * @return
     */
    public static int getdistanceofthispath(){
        return distanceofthispath;
    }

    /**
     * Processes already loaded and processed map, according to a* algorithm
     */
    private static void process(){
        if(openCells==null)
            return;
        openCells.add(mapa[startx][starty]);
        Cell current;

        while(true){
            current = openCells.poll();//cell with lowest f_cost

            if(current == null)
                break;

            closedCells[current.i][current.j] = true;
            if(current.equals(mapa[endx][endy])) {
                distanceofthispath = current.finalCost;
                return;
            }
            Cell neighborCell;

            int tmpcostchange;
            //spracovanie susedov vlavo
            if(current.i - 1 >= 0){
                neighborCell = mapa[current.i - 1][current.j];
                tmpcostchange = current.finalCost + V_H_cost;
                updateCostIfNeeded(current,neighborCell,tmpcostchange);
                if(current.j - 1 >= 0){//vlavo dolu
                    neighborCell = mapa[current.i - 1][current.j - 1];
                    tmpcostchange = current.finalCost + DiagonalCost;
                    updateCostIfNeeded(current,neighborCell,tmpcostchange);
                }
                if(current.j +1 < mapa[0].length){//vlavo hore
                    neighborCell = mapa[current.i-1][current.j+1];
                    tmpcostchange = current.finalCost + DiagonalCost;
                    updateCostIfNeeded(current,neighborCell,tmpcostchange);
                }

            }
            //hore
            if(current.j -1 >= 0){
                neighborCell = mapa[current.i][current.j-1];
                tmpcostchange = current.finalCost + V_H_cost;
                updateCostIfNeeded(current,neighborCell,tmpcostchange);
            }

            //dolu
            if(current.j + 1 < mapa[0].length){
                neighborCell = mapa[current.i][current.j+1];
                tmpcostchange = current.finalCost + V_H_cost;
                updateCostIfNeeded(current,neighborCell,tmpcostchange);
            }

            //doprava
            if(current.i +1 < mapa.length){
                neighborCell = mapa[current.i+1][current.j];
                tmpcostchange = current.finalCost + V_H_cost;
                updateCostIfNeeded(current,neighborCell,tmpcostchange);

                if (current.j - 1 >= 0){//doprava hore
                    neighborCell = mapa[current.i +1][current.j-1];
                    tmpcostchange = current.finalCost + DiagonalCost;
                    updateCostIfNeeded(current,neighborCell,tmpcostchange);
                }

                if (current.j + 1 < mapa[0].length){ //doprava dolu
                    neighborCell = mapa[current.i+1][current.j+1];
                    tmpcostchange = current.finalCost + DiagonalCost;
                    updateCostIfNeeded(current,neighborCell,tmpcostchange);
                }
            }
        }
    }
//
//    public static boolean correctPath(){
//        return(closedCells[endx][endy]);
//    }

}
