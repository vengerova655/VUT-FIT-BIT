package Store.StoreObject;

/**
 * Class representing a blocked aisle position on the store map.
 * @author xvosci00 Martin Voščinár
 */
public class Blockade extends StoreObject {
    /**
     * Constructor of a Blockade of default size - 1x1.
     * @param x x - coordinate of the blocked aisle position
     * @param y y - coordinate of the blocked aisle position
     */
    public Blockade(int x, int y) {
        super(x, y, StoreObject.Type.Blockade);
    }

    /**
     * Constructor of a Blockade of the given size.
     * @param x x - coordinate of the blocked aisle position
     * @param y y - coordinate of the blocked aisle position
     * @param sizeX horizontal size of the Blockade on the map
     * @param sizeY vertical size of the Blockade on the map
     */
    public Blockade(int x, int y, int sizeX, int sizeY) {
        super(x, y, sizeX, sizeY, StoreObject.Type.Blockade);
    }
}
