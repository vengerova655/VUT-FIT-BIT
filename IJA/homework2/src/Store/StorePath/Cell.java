package Store.StorePath;


/**
 * Class that represents one object in map( one point/square)
 *
 * @author xvenge01 Veronika Vengerova
 */
public class Cell {
    public int i,j;
    public Cell parent;
    public int heuristicCost;
    public int finalCost;

    /**
     * Constructor of Cell [x,y] coordinates of point on map
     *
     * @param i x coordinates
     * @param j y coordinates
     */
    public Cell(int i, int j){
        this.i = i;
        this.j = j;
    }
}
