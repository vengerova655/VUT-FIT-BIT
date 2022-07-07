package StoreController;

import Store.Cart.*;
import StoreView.*;
import Store.*;
import Store.StoreData.*;
import Store.StoreObject.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Queue;

/**
 * Class that should act as the Controller in MVC design.
 *
 * @author xvosci00 Martin Voščinár
 * @author xvenge01 Veronika Vengerova
 */
public class StoreController {
    private Store store;
    private StoreView view;
    // private FileHandler fileHandler;

    /**
     * Adds new cart to store
     * @param cart thecart we want to add
     */
    public void addCart(Cart cart){
        store.addCart(cart);
    }


    /**
     * Adds Order to list of Orders
     * @param order order which we want to add to system
     */
    public void addOrder(Order order){
        store.addOrder(order);
    }

    /**
     * Adds Order to Cart
     * @param order the order we wan to add to cart
     * @param cart the cart we want to add to
     */
    public void addOrder(Order order, Cart cart){
        store.addOrder(order, cart);
    }




    /**
     * Constructor that assigns Model and View to the controller attributes.
     * @param store Model as in MVC
     * @param view View as in MVC
     */
    public StoreController(Store store, StoreView view) {
        this.store = store;
        this.view = view;
    }

//    /**
//     * add
//     * @return
//     */
//    public Order addEmptyOrder(){
//        Order order = new Order();
//        store.addOrder(order);
//        return order;
//    }

    /**
     * Adds new request to order (element of order)
     * @param order order we want to add to
     * @param goods goods we want to add as request to order
     * @param number number of items of goods we want to request ito order
     */
    public void addToOrder(Order order, Goods goods, int number){
        store.addToOrder(order,goods,number);
    }

    /**
     * Moves Cart to new place on map
     * @param cart Cart we wan to move
     * @param x x-axis parameter
     * @param y y-axis parameter
     */
    public void moveCart(Cart cart, int x, int y){
       store.moveCart(cart,x,y);
    }

    /**
     * Gets array of Goods from order
     * @param order order whose array of goods we want to get
     * @return array of goods from order
     */
    public Goods[] getOrderGoods(Order order){
        return store.getOrderGoods(order);
    }



    // public boolean loadMapFile(String file) {
    //     this.fileHandler = new FileHandler(file);
    //     StoreObject[] map = this.fileHandler.getMap();
    //     int sizeX = this.fileHandler.getSizeX();
    //     int sizeY = this.fileHandler.getSizeY();
    //     return this.store.loadMap(map, sizeX, sizeY);
    // }

    // public void loadOrdersFile(String file) {}

    /**
     * Loads the given map using the associated store attribute.
     * @param map array with StoreObjects that represents a map of a store
     * @param sizeX the horizontal size of the map
     * @param sizeY the vertical size of the map
     * @return true if the map was loaded correctly
     */
    public boolean loadMapArray(StoreObject[] map, int sizeX, int sizeY) {
        return this.store.loadMap(map, sizeX, sizeY);
    }

    /**
     * Gets the closest path cart can move to in order to load cart's order
     * @param cart cart, whose path we  want to get
     * @return queue of x-axis and y-axis parameters stored in ArrayList
     */
    public Queue<ArrayList<Integer>> findClosestFromOrder(Cart cart) {
        Order order = store.getOrder(cart);
        if(order!=null)
            return store.findminpath(getOrderGoods(order), cart.getX(), cart.getY());
        else
            return null;
    }

    /**
     * Displays the closest path of cart to get one of the items from order it was assigned to.
     * @param cart cart whose path we want to find the shortest path of
     */
    public void displayClosestPath(Cart cart){
        System.out.println("CLOSEST PATH FROM ORDER:");
        Queue<ArrayList<Integer>> queue = findClosestFromOrder(cart);
        ArrayList<Integer> newcartmove = view.displaySolution(queue);//later will process differently moving cart
        if(newcartmove!=null)
            moveCart(cart,newcartmove.get(0),newcartmove.get(1));
    }

    /**
     * Removes order from cart cart will not have assigned order.
     * @param cart Cart whose order we want to remove
     */
    public void removeOrder(Cart cart){
            store.removeOrder(cart);
    }

    /**
     * Removes order from list of Orders
     * @param order the order we want to remove
     */
    public void removeOrder(Order order){
            store.removeOrder(order);
    }


    /**
     * Displays parameters of cart
     * @param cart Cart which we wan to represent
     */
    public void displayCart(Cart cart){//later cart ako parameter viacero v sklade
        view.displayCart(cart);
    }

    /**
     * Check if items of goods are in shelf.
     * @param goods array of goods we want to check
     * @param shelf shelf on which we want to check if the items are
     * @return true if at least one of goods is in shelf
     */
    public boolean ItemsInStore(Goods[] goods,Shelf shelf){
        for (Goods good: goods){
            Item[] items = store.getItems(good,shelf);
            if(items==null)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * Checks if the whole order was loaded to cart.
     * @param cart cart whose order we want to check
     * @return true if order was fully loaded to cart
     */
    public boolean OrderProcessed(Cart cart){
        return store.Orderprocessed(cart);
    }

    /**
     * Displays representation of order
     * @param order the order we want to represent
     */
    public void displayOrder(Order order){
        view.displayOrder(store.getOrderID(order),order);
    }

    /**
     * Gets HasMap representation of Order (only its elementary requests(Goods, number of items of good we want)).
     * @param order the order we want representation of
     * @return HasMap representation of order in form of key as goods, and value as number of items of good we want
     */
    public HashMap<Goods, Integer> getOrderHash(Order order){
        return order.getOrder();
    }

    /**
     * Loads the maximum number of Items from this position of cart (later implementation the cart will not move on shelf)
     * @param ord the order of which we want the most items of
     * @param cart the cart we want to load it into
     */
    public void loadMaxToCart(Order ord, Cart cart){
        store.loadMaxToCart(getOrderHash(ord),cart, cart.getX(), cart.getY());
    }

    public Order getOrder(Cart cart){
        return store.getOrder(cart);
    }
    /**
     * Get method for a shelf on a given position in the store map
     * @param x x coordinate of a shelf
     * @param y y coordinate of a shelf
     * @return shelf on x, y in the map loaded in store if it exists
     */
    public Shelf getShelf(int x, int y) {
        Shelf shelf;
        try {
            shelf = (Shelf) this.store.getStoreObject(x, y);
        }
        catch(ClassCastException e) {
            e.printStackTrace();
            return null;
        }
        return shelf;
    }

    /**
     * Fills the given shelf with a given amount of items.
     * @param goods Goods, which items should correspond to
     * @param shelf shelf where items should be loaded
     * @param count number of items to be loaded
     * @return true if all items were loaded successfuly
     */
    public boolean loadItems(Goods goods, Shelf shelf, int count) {
        this.view.displayItemsLoad(goods, shelf, count);
        return this.store.addItems(newItems(goods, count), shelf);
    }

    /**
     * Fills the given shelf with a given amount of items.
     * @param goodsName name of Goods, which items should correspond to
     * @param shelf shelf where items should be loaded
     * @param count number of items to be loaded
     * @return true if all items were loaded successfuly
     */
    public boolean loadItems(String goodsName, Shelf shelf, int count) {
        Goods goods = new Goods(goodsName);
        return loadItems(goods, shelf, count);
    }
//
//    public boolean loadItems(Goods goods, Shelf shelf, int count) {
//        return this.store.addItems(newItems(goods, count), shelf);
//    }
//
//    public boolean loadItems(String goodsName, int x, int y, int count) {
//        return loadItems(goodsName, getShelf(x, y), count);
//    }

    /**
     * Fills the shelf on the given position with a given amount of items.
     * @param goods Goods, which items should correspond to
     * @param x x - coordinate of a shelf where items should be loaded
     * @param y y - coordinate of a shelf where items should be loaded
     * @param count number of items to be loaded
     * @return true if all items were loaded successfuly
     */
    public boolean loadItems(Goods goods, int x, int y, int count) {
        return loadItems(goods, getShelf(x, y), count);
    }

    /**
     * Fills the shelf on the given position with a given amount of items.
     * @param goodsName name of Goods, which items should correspond to
     * @param x x - coordinate of a shelf where items should be loaded
     * @param y y - coordinate of a shelf where items should be loaded
     * @param count number of items to be loaded
     * @return true if all items were loaded successfuly
     */
    public boolean loadItems(String goodsName, int x, int y, int count) {
        return loadItems(goodsName, getShelf(x, y), count);
    }

    /**
     * Loads an empty map using the associated store attribute.
     * @param sizeX horizontal size of the map
     * @param sizeY vertical size of the map
     * @return true if the map was loaded successfuly
     */
    public boolean newMap(int sizeX, int sizeY) {
        return this.store.loadMap(new StoreObject[sizeX*sizeY], sizeX, sizeY);
    }

    /**
     * Creates a given number of items corresponding to given Goods
     * @param goods Goods which items should correspond to
     * @param count number of items to be created
     * @return array of created items
     */
    public Item[] newItems(Goods goods, int count) {
        Item[] items = new Item[count];
        for (int i = 0; i < count; i++) {
            items[i] = new Item(goods);
        }
        return items;
    }

    /**
     * Creates a given number of items corresponding to given Goods
     * @param goodsName name of Goods which items should correspond to
     * @param count number of items to be created
     * @return array of created items
     */
    public Item[] newItems(String goodsName, int count) {
        Goods goods = new Goods(goodsName);
        return newItems(goods, count);
    }

    /**
     * Creates a Shelf of the default size - 1x1 and on the given position.
     * @param x x - coordinate of the shelf
     * @param y y - coordinate of the shelf
     * @return true if the newly created Shelf was added to the given positon
     */
    public boolean newShelf(int x, int y) {
        Shelf shelf = new Shelf(x, y);
        return addStoreObject(shelf, x, y);
    }

    /**
     * Creates a Shelf of the given size on the given position in the map.
     * @param x x - coordinate of the shelf
     * @param y y - coordinate of the shelf
     * @param sizeX horizontal size of the shelf
     * @param sizeY vertical size of the shelf
     * @return true if the newly created Shelf was added to the given positon
     */
    public boolean newShelf(int x, int y, int sizeX, int sizeY) {
        Shelf shelf = new Shelf(x, y, sizeX, sizeY);
        return addStoreObject(shelf, x, y);
    }

    /**
     * Adds a StoreObjecy on the given position in the map.
     * @param obj StoreObject to be added
     * @param x x - coordinate where the object will be added
     * @param y y - coordinate where the object will be added
     * @return true if the given object was added to the given positon
     */
    public boolean addStoreObject(StoreObject obj, int x, int y) {
        this.view.displayObjectAdd(obj);
        return this.store.addStoreObject(obj, x, y);
    }

    /**
     * Adds a StoreObjecy on its position in the map.
     * @param obj StoreObject to be added
     * @return true if the given object was added to its positon
     */
    public boolean addStoreObject(StoreObject obj) {
        this.view.displayObjectAdd(obj);
        return this.store.addStoreObject(obj);
    }

    /**
     * Displays the store map.
     * <p>
     * Calls the {@link StoreView#displayMap} method.
     */
    public void displayMap() {
        view.displayMap(store.getMap(), store.getSizeX(), store.getSizeY());
    }

    /**
     * Displays list of objects in the map.
     * <p>
     * Calls {@link StoreView#displayObjects} method.
     */
    public void displayStoreObjects() {
        view.displayObjects(store.getMap(), store.getSizeX(), store.getSizeY());
    }

    /**
     * Displays list of objects in the map.
     * <p>
     * Calls {@link StoreView#displayObjects} method.
     */
    public void displayStoreObject(int x, int y) {
        view.displayObjects(store.getMap(), store.getSizeX(), store.getSizeY());
    }

    /**
     * Displays all of the Goods in the store.
     * <p>
     * Calls the {@link StoreView#displayGoods} method.
     */
    public void displayStoreGoods() {
        view.displayStoreGoods(this.store.getGoods());
    }

    /**
     * Displays the given Shelf with all of its Goods.
     * <p>
     * Calls the {@link StoreView#displayObject} method.
     *
     * @param shelf shelf whose items should be displayed
     */
    public void displayShelf(Shelf shelf) {
        view.displayObject(shelf);
    }

    /**
     * Displays the Shelf on the given position with all of its Goods.
     * <p>
     * Called method calls the {@link StoreView#displayObject} method.
     *
     * @param x x - coordinate of the shelf whose items should be displayed
     * @param y y - coordinate of the shelf whose items should be displayed
     */
    public void displayShelf(int x, int y) {
        this.displayObject(x, y);
    }

    /**
     * Displays the object on the given position in the map.
     * <p>
     * Calls the {@link StoreView#displayObject} method.
     *
     * @param x x - coordinate of the object which should be displayed
     * @param y y - coordinate of the object which should be displayed
     */
    public void displayObject(int x, int y) {
        StoreObject[] map = this.store.getMap();
        int sizeX = this.store.getSizeX();
        this.view.displayObject(map, sizeX, x, y);
    }

    /**
     * Displays the name and number of Goods on the given shelf in the store.
     * <p>
     * Calls the {@link StoreView#displayItemCountOnShelf} method.
     *
     * @param goods Goods of which item count should be displayed
     * @param shelf shelf whose item count should be displayed
     */
    public void displayItemCount(Goods goods, Shelf shelf) {
        int count = shelf != null ? shelf.getItems(goods).length : 0;
        view.displayItemCountOnShelf(goods, count, shelf);
    }

    /**
     * Displays the name and number of Goods on the given shelf position.
     * <p>
     * Called method calls the {@link StoreView#displayItemCountOnShelf} method.
     *
     * @param goods Goods of which item count should be displayed
     * @param x x - coordinate of the shelf whose item count should be displayed
     * @param y y - coordinate of the shelf whose item count should be displayed
     */
    public void displayItemCount(Goods goods, int x, int y) {
        this.displayItemCount(goods, getShelf(x, y));
    }

    /**
     * Displays the name and number of Goods on the given shelf position.
     * <p>
     * Called method calls the {@link StoreView#displayItemCountOnShelf} method.
     *
     * @param goodsName name of Goods of which item count should be displayed
     * @param shelf shelf whose item count should be displayed
     */
    public void displayItemCount(String goodsName, Shelf shelf) {
        this.displayItemCount(new Goods(goodsName), shelf);
    }

    /**
     * Displays the name and number of Goods on the given shelf position.
     * <p>
     * Called method calls the {@link StoreView#displayItemCountOnShelf} method.
     *
     * @param goodsName name of Goods of which item count should be displayed
     * @param x x - coordinate of the shelf whose item count should be displayed
     * @param y y - coordinate of the shelf whose item count should be displayed
     */
    public void displayItemCount(String goodsName, int x, int y) {
        this.displayItemCount(goodsName, getShelf(x, y));
    }

    /**
     * Removes given number of Goods items from the given Shelf.
     * <p>
     * Displays a message saying which items were removed and where from.
     * <p>
     * Calls the {@link StoreView#displayItemRemoval} method.
     *
     * @param goods Goods of which items should be removed
     * @param shelf shelf from which items should be removed
     * @param count number of items to be removed
     */
    public void removeItems(Goods goods, Shelf shelf, int count) {
        Item[] items = this.store.removeItems(goods, shelf, count);
        count = (shelf == null || items == null) ? 0 : items.length;

        this.view.displayItemRemoval(goods, shelf, count);
    }

    /**
     * Removes given number of Goods items from the Shelf on the given position.
     * <p>
     * Displays a message saying which items were removed and where from.
     * <p>
     * Called method calls the {@link StoreView#displayItemRemoval} method.
     *
     * @param goods Goods of which items should be removed
     * @param x x - coordinate of the shelf from which items should be removed
     * @param y y - coordinate of the shelf from which items should be removed
     * @param count number of items to be removed
     */
    public void removeItems(Goods goods, int x, int y, int count) {
        this.removeItems(goods, getShelf(x, y), count);
    }

    /**
     * Removes given number of Goods items from the given Shelf.
     * <p>
     * Displays a message saying which items were removed and where from.
     * <p>
     * Called method calls the {@link StoreView#displayItemRemoval} method.
     *
     * @param goodsName name of Goods of which items should be removed
     * @param shelf shelf from which items should be removed
     * @param count number of items to be removed
     */
    public void removeItems(String goodsName, Shelf shelf, int count) {
        Goods goods = new Goods(goodsName);
        this.removeItems(goods, shelf, count);
    }

    /**
     * Removes given number of Goods items from the Shelf on the given position.
     * <p>
     * Displays a message saying which items were removed and where from.
     * <p>
     * Called method calls the {@link StoreView#displayItemRemoval} method.
     *
     * @param goodsName name of Goods of which items should be removed
     * @param x x - coordinate of the shelf from which items should be removed
     * @param y y - coordinate of the shelf from which items should be removed
     * @param count number of items to be removed
     */
    public void removeItems(String goodsName, int x, int y, int count) {
        this.removeItems(goodsName, getShelf(x, y), count);
    }
}
