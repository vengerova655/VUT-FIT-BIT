package Store.StoreData;

import Store.StoreObject.*;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * Class that encapsulates the store map representation and associated methods.
 * @author xvosci00 Martin Voščinár
 */
public class StoreMap {
    private int sizeX, sizeY;
    private int LOWLIMIT = 3; // lowest limit of map width and height
    private ArrayList<StoreObject> map;

    /**
     * Creates an empty map of the given size.
     * @param sizeX horizontal size of the map
     * @param sizeY vertical size of the map
     */
    public StoreMap(int sizeX, int sizeY) {
        this(new StoreObject[sizeX*sizeY], sizeX, sizeY);
    }

    /**
     * Creates a map of the given size from the given map array.
     * @param map array which represents a map that should be loaded
     * @param sizeX horizontal size of the map
     * @param sizeY vertical size of the map
     */
    public StoreMap(StoreObject[] map, int sizeX, int sizeY) {
        if (map.length != sizeX*sizeY) {
            throw new IndexOutOfBoundsException("Map size does not match");
        }
        checkSize(sizeX, sizeY);
        this.map = new ArrayList<StoreObject>(Arrays.asList(map));
        this.sizeX = sizeX;
        this.sizeY = sizeY;
    }

    /**
     * Adds a store object on the given position on the map.
     * @return true if the object was added
     */
    public boolean add(StoreObject object, int x, int y) {
        return add(object);
    }

    /**
     * Adds a store object on the given position on the map.
     * @return true if the object was added
     */
    public boolean add(StoreObject object) {
        int x = object.getX();
        int y = object.getY();
        checkIndex(object);

        for (int i = x; i < x + object.getSizeX(); i++) {
            for (int j = y; j < y + object.getSizeY(); j++) {
                this.map.set(i + j*this.sizeX, object);
            }
        }

        return true;
    }

    /**
     * Gets a store object on the given position on the map.
     * @return store object on the given position or null
     */
    public StoreObject get(int x, int y) {
        checkIndex(x, y);
        return this.map.get(x + y*this.sizeX);
    }

    /**
     * Gets the loaded map array or an empty one if it is not loaded.
     * @return the map array
     */
    public StoreObject[] getMap() {
        return this.map.toArray(new StoreObject[0]);
    }

    /**
     * Gets the horizontal size of the map.
     * @return horizontal size of the map
     */
    public int getSizeX() {
        return this.sizeX;
    }

    /**
     * Gets the vertical size of the map.
     * @return vertical size of the map
     */
    public int getSizeY() {
        return this.sizeY;
    }

    /**
     * Determines if the store map is loaded.
     * @return true if the store map is loaded - is not empty
     */
    public boolean isLoaded() {
        return !isEmpty();
    }

    /**
     * Determines if the store map is empty.
     * @return true if the store map is empty
     */
    public boolean isEmpty() {
        return this.map == null || this.map.isEmpty();
    }

    /**
     * Clears the store map.
     */
    public void clear() {
        this.map = new ArrayList<StoreObject>(sizeX*sizeY);
    }

    /**
     * Checks if the given position is inside of the map.
     * @param x x - coordinate
     * @param y y - coordinate
     * @return true if the given position is not out of bounds
     * @throws IndexOutOfBoundsException if the given position is out of bounds
     */
    private boolean checkIndex(int x, int y) throws IndexOutOfBoundsException {
        if (x >= this.sizeX || y >= this.sizeY) {
            throw new IndexOutOfBoundsException(
                                "Map index ["+ x + ","+y +"] is out of bounds");
        }
        return true;
    }

    /**
     * Checks if the given object's position is inside of the map.
     * @param o object with a position attribute
     * @return true if the given object is not out of bounds
     * @throws IndexOutOfBoundsException if the given object is out of bounds
     */
    private boolean checkIndex(StoreObject o) throws IndexOutOfBoundsException {
        int x = o.getX();
        int y = o.getY();
        int sizeX = o.getSizeX();
        int sizeY = o.getSizeY();
        StoreObject.Type type = o.getType();
        if (x >= this.sizeX || y >= this.sizeY) {
            throw new IndexOutOfBoundsException(type+" position out of bounds");
        }
        if (x + sizeX > this.sizeX || y + sizeY > this.sizeY) {
            throw new IndexOutOfBoundsException(type+" size is out of bounds: "+
                "Map size: "+this.sizeX+", "+this.sizeY+
                " Object dimensions: "+x+"-"+(x+sizeX)+", " + y +"-"+(y+sizeY));
        }
        return true;
    }

    /**
     * Checks if the size of the map that should be loaded meets the low limit.
     * @param sizeX horizontal size of the map
     * @param sizeY vertical size of the map
     * @return true if the map size meets the low limit
     */
    private boolean checkSize(int sizeX, int sizeY) {
        return sizeX >= this.LOWLIMIT && sizeY >= this.LOWLIMIT;
    }
}
