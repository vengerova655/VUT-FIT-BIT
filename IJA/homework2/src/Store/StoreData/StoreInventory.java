package Store.StoreData;

import Store.StoreObject.*;

import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;

/**
 * Class that encapsulates the store inventory data.
 * <p>
 * It stores a collection of shelves and a collection of items for each Goods.
 * 
 * @author xvosci00 Martin Voščinár
 */
public class StoreInventory {
    private HashMap<Goods, HashSet<Shelf>> shelves;
    private HashMap<Goods, HashSet<Item>> items;

    /**
     * Constructor for an empty store inventory.
     */
    public StoreInventory() {
        this.shelves = new HashMap<Goods, HashSet<Shelf>>();
        this.items = new HashMap<Goods, HashSet<Item>>();
    }

    /**
     * Adds the given item to the given shelf and to the inventory.
     * @param item the given item that should be added
     * @param shelf shelf onto which the item should be loaded
     * @return true if the item was added to the shelf and to the inventory
     */
    public boolean addItem(Item item, Shelf shelf) {
        if (item == null || shelf == null || item.getGoods() == null) {
            return false;
        }
        return addToShelf(item, shelf) && addToItems(item);
    }

    /**
     * Adds the array of given items to the given shelf and to the inventory.
     * <p>
     * Calls the {@link #addItem} method.
     * 
     * @param items array of the given items that should be added
     * @param shelf shelf onto which the items should be loaded
     * @return true if the items were added to the shelf and to the inventory
     */
    public boolean addItems(Item[] items, Shelf shelf) {
        for (Item item : items) {
            if (!(addItem(item, shelf))) {
                return false;
            }
        }
        return true;
    }

    /**
     * Adds the array of given items to the given shelf.
     * <p>
     * Calls the {@link Shelf#addItem} method.
     * 
     * @param item array of the given items that should be added
     * @param shelf shelf onto which the items should be loaded
     * @return true if the items were added to the shelf
     */
    private boolean addToShelf(Item item, Shelf shelf) {
        Goods goods = item.getGoods();
        HashSet<Shelf> set = this.shelves.get(goods);

        if (set == null) {
            this.shelves.put(goods, new HashSet<Shelf>());
            set = this.shelves.get(goods);
        }
        set.add(shelf);

        return shelf.addItem(item);
    }

    /**
     * Adds the array of given items to the inventory.
     * 
     * @param item array of the given items that should be added
     * @return true if the items were added to the inventory
     */
    private boolean addToItems(Item item) {
        Goods goods = item.getGoods();
        HashSet<Item> set = this.items.get(goods);

        if (set == null) {
            this.items.put(goods, new HashSet<Item>());
            set = this.items.get(goods);
        }
        set.add(item);

        return true;
    }

    /**
     * Removes an item of given Goods from the shelf and from the inventory.
     * @param goods Goods of which an item should be removed
     * @param shelf shelf from which the item should be removed
     * @return the removed item
     */
    public Item removeItem(Goods goods, Shelf shelf) {
        Item i = shelf != null ? shelf.removeItem(goods) : null;
        return (i != null && this.items.get(i.getGoods()).remove(i)) ? i : null;
    }

    /**
     * Removes the given count of items of given Goods from the given Shelf.
     * <p>
     * Calls the {@link StoreInventory#removeItem} method.
     * 
     * @param goods Goods of which the given count of items should be removed
     * @param shelf shelf from which the items should be removed
     * @param count number of items that should be removed
     * @return the array of removed items
     */
    public Item[] removeItems(Goods goods, Shelf shelf, int count) {
        Item[] items = new Item[count];
        for (int i = 0; i < count; i++) {
            items[i] = removeItem(goods, shelf);
        }
        return items;
    }

    /**
     * Removes the given array of items just from the inventory.
     * <p>
     * Expects that they were removed from its shelf before.
     * 
     * @param items given array of items
     * @return true if the items were successfuly removed
     */
    public boolean removeItemsFromInventory(Item[] items) {
        for(Item item : items) {
            this.items.get(item.getGoods()).remove(item);
        }
        return true;
    }

    /**
     * Gets all of the Goods in the store inventory.
     * 
     * @return array of all of the goods on the store
     */
    public Goods[] getGoods() {
        Set<Goods> set = this.shelves.keySet();
        return set != null ? set.toArray(new Goods[0]) : new Goods[0];
    }

    /**
     * Gets all of the Goods on the given shelf.
     * 
     * @return array of all of the goods on the shelf
     */
    public Goods[] getGoods(Shelf shelf) {
        return shelf != null ? shelf.getGoods() : new Goods[0];
    }

    /**
     * Gets the Shelf on the given position and containing the given goods.
     * @param x x - coordinate of a Shelf
     * @param y y - coordinate of a Shelf
     * @param goods given Goods
     * @return Shelf on the given position and containing the given goods
     */
    public Shelf getShelf(int x, int y, Goods goods) { // TODO remove
        Shelf[] shelves = getShelves(goods);
        for(Shelf shelf : shelves){
            if (shelf.getX() == x && shelf.getY() == y){
                return shelf;
            }
        }
        return null;
    }

    /**
     * Gets the all of the shelves containing the given goods.
     * @param goods given Goods
     * @return array of the shelves containing the given goods
     */
    public Shelf[] getShelves(Goods goods) {
        HashSet<Shelf> set = this.shelves.get(goods);
        return set != null ? set.toArray(new Shelf[0]) : new Shelf[0];
    }

    /**
     * Gets all of the items of the given Goods in the store inventory.
     * @param goods given Goods of which items should be returned
     * @return array of all of the items of the given Goods in the store
     */
    public Item[] getItems(Goods goods) {
        HashSet<Item> set = this.items.get(goods);
        return set != null ? set.toArray(new Item[0]) : new Item[0];
    }

    /**
     * Gets all of the items of the given Goods on the given Shelf.
     * @param goods given Goods of which items should be returned
     * @param shelf given Shelf on which items are located
     * @return array of all of the items of the given Goods on the given Shelf
     */
    public Item[] getItems(Goods goods, Shelf shelf) {
        return shelf.getItems(goods);
    }

    /**
     * Clears the store inventory.
     */
    public void clear() {
        this.items.clear();
        this.shelves.clear();
    }
}
