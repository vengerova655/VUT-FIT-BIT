package Store.StoreObject;

/**
 * Class representing a pickup spot on the store map.
 * @author xvosci00 Martin Voščinár
 */
public class PickupSpot extends StoreObject {
    boolean occupied = false;
    boolean open = true;

    /**
     * Constructor of a pickup spot of default size - 1x1 on the map.
     * @param x x - coordinate of the pickup spot on the store map
     * @param y y - coordinate of the pickup spot on the store map
     */
    public PickupSpot(int x, int y) {
        super(x, y, StoreObject.Type.PickupSpot);
    }

    /**
     * Constructor of a pickup spot of the given size on the map.
     * @param x x - coordinate of the pickup spot on the store map
     * @param y y - coordinate of the pickup spot on the store map
     * @param sizeX horizontal size of the PickupSpot on the map
     * @param sizeY vertical size of the PickupSpot on the map
     */
    public PickupSpot(int x, int y, int sizeX, int sizeY) {
        super(x, y, sizeX, sizeY, StoreObject.Type.PickupSpot);
    }

    /**
     * Determines if the PickupSpot is open
     * @return true if the PickupSpot is open
     */
    public boolean isOpen() {
        return this.open;
    }

    /**
     * Determines if the PickupSpot is being used
     * @return true if the PickupSpot is being used
     */
    public boolean isOccupied() {
        return this.occupied;
    }

    /**
     * Sets the open attribute to true.
     */
    public void setOpen() {
        this.open = true;
    }

    /**
     * Sets the open attribute to false.
     */
    public void setClosed() {
        this.open = false;
    }

    /**
     * Sets the occupied attribute to true.
     */
    public void setOccupied() {
        this.occupied = true;
    }

    /**
     * Sets the occupied attribute to false.
     */
    public void setUnOccupied() {
        this.occupied = false;
    }
}
