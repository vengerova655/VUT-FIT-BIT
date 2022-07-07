package Store;

import Store.Cart.Cart;
import Store.Cart.CartHandler;
import Store.Cart.Order;
import Store.StoreData.*;
import Store.StoreObject.*;
import Store.StorePath.*;

/**
 * Class representing the Model in MVC design.
 * @author xvosci00 Martin Voščinár
 * @author xvenge01 Veronika Vengerova
 */
import java.util.*;

public class Store {
    private StoreInventory inv;
    private StoreMap map;

    /**
     * Constructor for an empty Store.
     * <p>
     * Calls the {@link StoreInventory#StoreInventory} constructor.
     */
    public Store() {
        this.inv = new StoreInventory();
    }

    /**
     * Constructor for an empty Store of given size proportions.
     * <p>
     * Calls the {@link StoreMap#StoreMap(int sizeX, int sizeY)} constructor.
     *
     * @param sizeX horizontal size of the store map
     * @param sizeY vertical size of the store map
     */
    public Store(int sizeX, int sizeY) {
        this();
        this.map = new StoreMap(sizeX, sizeY);
    }

    /**
     * Constructor for a Store with a given map of given size proportions.
     * <p>
     * Calls the {@link #loadMap} method.
     *
     * @param map store map to be loaded
     * @param sizeX horizontal size of the store map
     * @param sizeY vertical size of the store map
     */
    public Store(StoreObject[] map, int sizeX, int sizeY) {
        this();
        loadMap(map, sizeX, sizeY);
    }


    /**
     * Adds order to system.
     * @param order order we want  to add
     */
    public void addOrder(Order order){
        CartHandler.addOrder(order);
    }

    /**
     * Adds new request to order
     *
     * @param order the order we want to add new request to
     * @param goods goods of the request
     * @param number number of goods of request
     */
    public void addToOrder(Order order, Goods goods, int number){
        CartHandler.addToOrder(order,goods,number);
    }

    /**
     * Moves Cart to new coordinates.
     *
     * @param cart which cart we want to move
     * @param x x-coordinates where we want to move the cart to
     * @param y y-coordinates where we want to move the cart to
     */
    public void moveCart(Cart cart, int x, int y){
        CartHandler.movecart(cart,x,y);
    }

    /**
     * Gets Array of goods requested in order.
     *
     * @param order order whose goods we want to get
     * @return arrayof goods
     */
    public Goods[] getOrderGoods(Order order){
//        return this.order.getArrGoods();
        return CartHandler.getOrderGoods(order);
    }


    /**
     * Loads the given store map.
     * <p>
     * Calls the {@link StoreMap} constructor.
     *
     * @param map store map to be loaded
     * @param sizeX horizontal size of the store map
     * @param sizeY vertical size of the store map
     * @return true
     */
    public boolean loadMap(StoreObject[] map, int sizeX, int sizeY) {
        this.map = new StoreMap(map, sizeX, sizeY);
        return true;
    }
    public void addCart(Cart cart){
        CartHandler.addCart(cart);
//        this.cart = cart;//dalej pridat do zozznamu cartov
    }

    /**
     * Gets the loaded map array or an empty one if it is not loaded.
     * <p>
     * Calls the {@link StoreMap#getMap} method.
     *
     * @return the map array
     */
    public StoreObject[] getMap() {
        return mapIsLoaded() ? this.map.getMap() : new StoreObject[0];
    }

    public void addOrder(Order order,Cart cart){
        CartHandler.addOrdertoCart(order,cart);
    }

    /**
     * Gets the horizontal size of the map it is loaded or 0 if not.
     * <p>
     * Calls the {@link StoreMap#getSizeX} method.
     *
     * @return horizontal size of the map
     */
    public int getSizeX() {
        return mapIsLoaded() ? this.map.getSizeX() : 0;
    }

    /**
     * Gets the vertical size of the map it is loaded or 0 if not.
     * <p>
     * Calls the {@link StoreMap#getSizeY} method.
     *
     * @return vertical size of the map
     */
    public int getSizeY() {
        return mapIsLoaded() ? this.map.getSizeY() : 0;
    }

    /**
     * Gets a store object on the given position on the map if it is loaded.
     * <p>
     * Calls the {@link StoreMap#get(int x, int y)} method.
     *
     * @return store object on the given position or null
     */
    public StoreObject getStoreObject(int x, int y) {
        return mapIsLoaded() ? this.map.get(x, y) : null;
    }

    /**
     * Adds a store object on the given position on the map if it is loaded.
     * <p>
     * Calls the {@link StoreMap#add(StoreObject object, int x, int y)} method.
     *
     * @return true if the object was added
     */
    public boolean addStoreObject(StoreObject object, int x, int y) {
        return mapIsLoaded() ? this.map.add(object, x, y) : false;
    }

    /**
     * Adds a store object on its position on the map if it is loaded.
     * <p>
     * Calls the {@link StoreMap#add(StoreObject object)} method.
     *
     * @return true if the object was added
     */
    public boolean addStoreObject(StoreObject object) {
        return mapIsLoaded() ? this.map.add(object) : false;
    }

    /**
     * Determines if the store map is loaded.
     * <p>
     * Calls the {@link StoreMap#isLoaded} method.
     *
     * @return true if the store map is loaded
     */
    public boolean mapIsLoaded() {
        return this.map != null && this.map.isLoaded();
    }

    /**
     * Adds the given item to the inventory and to the given shelf.
     * <p>
     * Calls the {@link StoreInventory#addItem} method.
     *
     * @param item the given item that should be added
     * @param shelf shelf onto which the item should be loaded
     * @return true if the item was added to the shelf and to the inventory
     */
    public boolean addItem(Item item, Shelf shelf) {
        return this.inv.addItem(item, shelf);
    }

    /**
     * Adds the array of given items to the inventory and to the given shelf.
     * <p>
     * Calls the {@link StoreInventory#addItems} method.
     *
     * @param items array of the given items that should be added
     * @param shelf shelf onto which the items should be loaded
     * @return true if the items were added to the shelf and to the inventory
     */
    public boolean addItems(Item[] items, Shelf shelf) {
        return this.inv.addItems(items, shelf);
    }

    /**
     * Removes an item of given Goods from the shelf and from the inventory.
     * <p>
     * Calls the {@link StoreInventory#removeItem} method.
     *
     * @param goods Goods of which an item should be removed
     * @param shelf shelf from which the item should be removed
     * @return the removed item
     */
    public Item removeItem(Goods goods, Shelf shelf) {
        return this.inv.removeItem(goods, shelf);
    }

    /**
     * Removes the given count of items of given Goods from the given Shelf.
     * <p>
     * Calls the {@link StoreInventory#removeItems} method.
     *
     * @param goods Goods of which the given count of items should be removed
     * @param shelf shelf from which the items should be removed
     * @param count number of items that should be removed
     * @return the array of removed items
     */
    public Item[] removeItems(Goods goods, Shelf shelf, int count) {
        return this.inv.removeItems(goods, shelf, count);
    }

    /**
     * Gets the order assigned to cart.
     *
     * @param cart we want to the order assigned to
     * @return order assigned to cart
     */
    public Order getOrder(Cart cart){
        return CartHandler.getOrder(cart);
    }

    /**
     * Gets id of order.
     *
     * @param order
     * @return
     */
    public int getOrderID(Order order){
    return CartHandler.getOrderID(order);
}


    /**
     * Removes order of cart (will not have assigned anymore).
     *
     * @param cart cart whose order we want to remove
     */
    public void removeOrder(Cart cart){
        Order order = CartHandler.getOrder(cart);
        CartHandler.removeOrder(order);
    }

    /**
     * Remove order from system.
     * @param order order we want to remove
     */
    public void removeOrder(Order order){
            CartHandler.removeOrder(order);
    }

    /**
     * Loads the maximum of order items into cart
     * @param fromorder HasMap of goods and number of goods we want
     * @param cart cart we want to move it into
     * @param x x-axis of shelf we want to load from
     * @param y y-axis of shelf we want to load from
     */
    public void loadMaxToCart(HashMap<Goods, Integer> fromorder, Cart cart, int x, int y) {
        for (Goods goods1 : fromorder.keySet()) {
            Shelf shelf = inv.getShelf(x, y, goods1);
            if (shelf != null) {
                Item[] items = (inv.getItems(goods1, shelf));
                for (Item items1 : items) {
                    if (cart.numofItems(goods1) == fromorder.get(goods1)) {
                        break;
                    }
                        CartHandler.loadToCart(cart, items1);
                        inv.removeItem(goods1, shelf);

                    }
                }
            }

        }

    /**
     * Checks, if the order of cart is fully loaded into cart
     * @param cart cart whose order we want to check
     * @return true if order was fully loaded into cart
     */
    public boolean Orderprocessed(Cart cart){
            return CartHandler.orderprocessed(cart);
    }

    //sx, sy z kontorlru poloha cartu

    /**
     *  Finds minimal path from coordinates[x,y] to get one of goods.
     *  <p>
     *      Always chooses the shortest distance from all goods.
     *  </p>
     * @param goods goods we want to find the shortest path to
     * @param sx x-coordinates from which place we want to find the shortest apth to
     * @param sy y-coordinates from which place we want to find the shortest apth to
     * @return Queue of x,y coordinates saved in ArrayList [x,y]
     */
    public  Queue<ArrayList<Integer>> findminpath(Goods[] goods,int sx, int sy){
        int min=Integer.MAX_VALUE;
        Queue<ArrayList<Integer>> minqueue = new LinkedList<ArrayList<Integer>>();
        for(Goods goods1: goods){
            Shelf[] shelves = inv.getShelves(goods1);
            for (Shelf shelf: shelves){

                Queue<ArrayList<Integer>> queue = new LinkedList<ArrayList<Integer>>();
                queue = getPath(sx, sy,shelf.getX(),shelf.getY());
                if(shelf.getItems(goods1).length!=0)
                    if (min>=queue.peek().get(0)){
                        min = queue.poll().get(0);
                        Queue<ArrayList<Integer>> kju = new LinkedList<ArrayList<Integer>>();
                        while(queue!=null) {
                            ArrayList<Integer> tmp = queue.poll();
                            if(tmp==null)
                                break;
                            kju.add(tmp);
                        }
                        minqueue=kju;
                    }
            }
        }
        return minqueue;
    }


    /**
     * Removes the given array of items just from the inventory.
     * <p>
     * Expects that they were removed from its shelf before.
     * <p>
     * Calls the {@link StoreInventory#removeItemsFromInventory} method.
     *
     * @param items given array of items
     * @return true if the items were successfuly removed
     */
    public boolean removeItemsFromInventory(Item[] items) {
        return this.inv.removeItemsFromInventory(items);
    }


    /**
     * Gets all of the Goods in the store.
     * <p>
     * Calls the {@link StoreInventory#getGoods()} method.
     *
     * @return array of all of the goods on the store
     */

    public Goods[] getGoods() {
        return this.inv.getGoods();
    }

    /**
     * Gets all of the Goods in the store on the given shelf.
     * <p>
     * Calls the {@link StoreInventory#getGoods(Shelf shelf)} method.
     *
     * @return array of all of the goods on the shelf
     */
    public Goods[] getGoods(Shelf shelf) {
        return this.inv.getGoods(shelf);
    }

    /**
     * Gets the all of the shelves containing the given goods.
     * <p>
     * Calls the {@link StoreInventory#getShelves} method.
     *
     * @param goods given Goods
     * @return array of the shelves containing the given goods
     */
    public Shelf[] getShelves(Goods goods) {
        return this.inv.getShelves(goods);
    }

    /**
     * Gets all of the items of the given Goods in the store.
     * <p>
     * Calls the {@link StoreInventory#getItems(Goods goods)} method.
     *
     * @return array of all of the items of the given Goods in the store
     */
    public Item[] getItems(Goods goods) {
        return this.inv.getItems(goods);
    }


    /**
     * Gets Shortes path from 1. coordinates to 2. coordinates.
     * @param sx x-coordinates of starting poitn
     * @param sy y-coordinates of starting poitn
     * @param ex x-coordinates of ending poitn
     * @param ey x-coordinates of ending poitn
     * @return queue of arrays of x,y coordinates of shortest path
     */
    public Queue<ArrayList<Integer>> getPath(int sx, int sy, int ex, int ey) {
        return StorePath.getPath(this.map, ex, ey, sx, sy);
    }

    /**
     * Gets all of the items of the given Goods on the given Shelf.
     * <p>
     * Calls {@link StoreInventory#getItems(Goods goods, Shelf shelf)}.
     *
     * @return array of all of the items of the given Goods on the given Shelf
     */
    public Item[] getItems(Goods goods, Shelf shelf) {
        return this.inv.getItems(goods, shelf);
    }

    /**
     * Clears the store inventory and map.
     * <p>
     * Calls {@link StoreInventory#clear}.
     * Calls {@link StoreMap#clear}.
     */
    public void clear() {
        this.inv.clear();
        this.map.clear();
    }
}
