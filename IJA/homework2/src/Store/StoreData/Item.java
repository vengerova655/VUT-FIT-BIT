package Store.StoreData;

import Store.StoreObject.*;

/**
 * Class that represents an item in the store.
 * <p>
 * In stores its Goods, optional name and Shelf where it is located.
 * 
 * @author xvosci00 Martin Voščinár
 */
public class Item {
    private Goods goods;
    private String name;
    private Shelf shelf;

    /**
     * Contructor for the item of goods with an empty name attribute.
     * @param goods Goods which the item corresponds to
     */
    public Item(Goods goods) {
        this.goods = goods;
        this.name = "";
    }

    /**
     * Contructor for the item of goods with the given name attribute.
     * @param name optional name attribute
     * @param goods Goods which the item corresponds to
     */
    public Item(String name, Goods goods) {
        this(goods);
        this.name = name;
    }

    /**
     * Gets the corresponding Goods for the item.
     * @return corresponding Goods
     */
    public Goods getGoods() {
        return this.goods;
    }

    /**
     * Gets the optional name attribute.
     * @return optional name attribute String
     */
    public String getName() {
        return this.name;
    }

    /**
     * Gets the shelf on which the item located.
     * @return the shelf on which it is located
     */
    public Shelf getShelf() {
        return this.shelf;
    }

    /**
     * Sets the optional name attribute.
     * @param name name String
     * @return true
     */
    public boolean setName(String name) {
        this.name = name;
        return true;
    }

    /**
     * Sets the shelf attribute to the given shelf.
     * @param shelf Shelf on which item will be located
     * @return true
     */
    public boolean setShelf(Shelf shelf) {
        this.shelf = shelf;
        return true;
    }

    /**
     * Overrides the default method to return the goods name and name attribute.
     * @return Goods name and optional item name String
     */
    @Override
    public String toString() {
        return this.goods + " " + this.name;
    }

    /**
     * Removes the optional name of the item.
     */
    public void clearName() {
        this.name = "";
    }
}
