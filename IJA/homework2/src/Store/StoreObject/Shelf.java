package Store.StoreObject;

import Store.StoreData.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Set;

/**
 * Class representing a shelf on the store map.
 * @author xvosci00 Martin Voščinár
 */
public class Shelf extends StoreObject {
    private HashMap<Goods, ArrayList<Item>> items;

    /**
     * Constructor of an empty Shelf of default size - 1x1 on the map.
     * @param x x - coordinate of the Shelf
     * @param y y - coordinate of the Shelf
     */
    public Shelf(int x, int y) {
        super(x, y, StoreObject.Type.Shelf);
        this.items = new HashMap<Goods, ArrayList<Item>>();
    }

    /**
     * Constructor of an empty Shelf of the given size on the map.
     * @param x x - coordinate of the Shelf
     * @param y y - coordinate of the Shelf
     * @param sizeX horizontal size of the Shelf on the map
     * @param sizeY vertical size of the Shelf on the map
     */
    public Shelf(int x, int y, int sizeX, int sizeY) {
        super(x, y, sizeX, sizeY, StoreObject.Type.Shelf);
        this.items = new HashMap<Goods, ArrayList<Item>>();
    }

    /**
     * Constructor of a Shelf of default size - 1x1 and loaded with given items.
     * @param x x - coordinate of the Shelf
     * @param y y - coordinate of the Shelf
     * @param items array of items which should be loaded onto the Shelf
     */
    public Shelf(int x, int y, Item[] items) {
        this(x, y);
        for (Item item : items) {
            addItem(item);
        }
    }

    /**
     * Constructor of a Shelf of of the given size and loaded with given items.
     * @param x x - coordinate of the Shelf
     * @param y y - coordinate of the Shelf
     * @param sizeX horizontal size of the Shelf on the map
     * @param sizeY vertical size of the Shelf on the map
     * @param items array of items which should be loaded onto the Shelf
     */
    public Shelf(int x, int y, int sizeX, int sizeY, Item[] items) {
        this(x, y, sizeX, sizeY);
        for (Item item : items) {
            addItem(item);
        }
    }

    /**
     * Adds an item to the shelf.
     * @param item which should be added to the shelf
     * @return true if the given item was successfuly loaded onto the shelf
     */
    public boolean addItem(Item item) {
        Goods goods = item.getGoods();
        ArrayList<Item> list = this.items.get(goods);
    
        if (list == null) {
            this.items.put(goods, new ArrayList<Item>());
            list = this.items.get(goods);
        }

        return list.add(item) && item.setShelf(this);
    }

    /**
     * Removes an Item of Goods from the shelf.
     * @param goods Goods of which an item should be removed
     * @return an item that was removed
     */
    public Item removeItem(Goods goods) {
        ArrayList<Item> list = this.items.get(goods);
        return (list != null && !list.isEmpty()) ? list.remove(0) : null;
    }

    /**
     * Gets an array of Goods on the Shelf.
     * @return array of Goods on the Shelf
     */
    public Goods[] getGoods() {
        Set<Goods> set = this.items.keySet();
        return !set.isEmpty() ? set.toArray(new Goods[0]) : new Goods[0];
    }

    /**
     * Gets an array of Items of Goods on the Shelf.
     * @return array of Items of Goods on the Shelf
     */
    public Item[] getItems(Goods goods) {
        ArrayList<Item> list = this.items.get(goods);
        return list != null ? list.toArray(new Item[0]) : new Item[0];
    }

    /**
     * Overrides the default method to return "Shelf" String and its Goods.
     * @return type attribute String representation "Shelf" and shelf Goods
     */
    @Override
    public String toString() {
        return super.toString() + Arrays.toString(this.getGoods());
    }

    /**
     * Removes all items from the Shelf.
     */
    public void clear() {
        this.items.clear();
    }
}
