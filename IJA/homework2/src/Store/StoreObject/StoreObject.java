package Store.StoreObject;

/**
 * Abstract class representing a general object on the store map.
 * @author xvosci00 Martin Voščinár
 */
public abstract class StoreObject {
    protected int posX, posY;
    protected int sizeX, sizeY;
    protected Type type;

    /**
     * Constructor that assigns the position and type to the created object.
     * <p>
     * Assigns the default size of 1x1.
     * @param x x - coordinate on the map
     * @param y y - coordinate on the map
     * @param type type of the object
     */
    public StoreObject(int x, int y, Type type) {
        this.posX = x;
        this.posY = y;
        this.sizeX = 1;
        this.sizeY = 1;
        this.type = type;
    }

    /**
     * Constructor that assigns the position, type and size to the object.
     * @param x x - coordinate on the map
     * @param y y - coordinate on the map
     * @param type type of the object
     * @param sizeX horizontal size of the object on the map
     * @param sizeY vertical size of the object on the map
     */
    public StoreObject(int x, int y, int sizeX, int sizeY, Type type) {
        this(x, y, type);
        this.sizeX = sizeX;
        this.sizeY = sizeY;
    }

    /**
     * Gets the object type.
     * @return type of the object
     */
    public Type getType() {
        return this.type;
    }

    /**
     * Gets the object x - coordinate.
     * @return x - coordinate of the object
     */
    public int getX() {
        return this.posX;
    }

    /**
     * Gets the object y - coordinate.
     * @return y - coordinate of the object
     */
    public int getY() {
        return this.posY;
    }

    /**
     * Gets the horizontal size of the object on the map.
     * @return horizontal size of the object
     */
    public int getSizeX() {
        return this.sizeX;
    }

    /**
     * Gets the vertical size of the object on the map.
     * @return vertical size of the object
     */
    public int getSizeY() {
        return this.sizeY;
    }

    /**
     * Overrides the default method to return the type attribute String.
     * @return type attribute String representation
     */
    @Override
    public String toString() {
        return this.type.toString();
    }

    /**
     * Type of the object on the store map.
     * @author xvosci00 Martin Voščinár
     */
    public enum Type {
        PickupSpot("Pickup Spot"), Shelf("Shelf"), Blockade("Blockade");
        private final String str;
        Type(String str) {
            this.str = str;
        }
        @Override
        public String toString() {
            return this.str;
        }
    }    
}
