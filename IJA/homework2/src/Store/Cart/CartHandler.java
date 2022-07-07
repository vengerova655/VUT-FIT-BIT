package Store.Cart;

import Store.StoreData.Goods;
import Store.StoreData.Item;

import java.util.*;

/**
 * The class that handles work with carts and orders.
 *
 * @author xvenge01 Veronika Vengerova
 */
public final class CartHandler {
    private final static HashMap<Cart, Order> carthandler = new HashMap<>(); //later create hashset form order more orders to 1 cart
    private final static HashMap<Integer,Order> orders = new HashMap<>();


    /**
     * Adds order to list of orders.
     *
     * @param order order we want to add
     */
    public static void addOrder(Order order){
        orders.put(orders.keySet().size()+1,order);
    }

    /**
     * Gets the id of order in list of all orders.
     *
     * @param order the order whose id(number) we want to get
     * @return the id of order(integer)
     */
    private static int getKeyOfOrder(Order order){
        for(int ordindex : orders.keySet()){
            if(order.equals(orders.get(ordindex))){
                return ordindex;
            }
        }
        return -1;
    }

    /**
     * Removes cart from store.
     * @param cart cart we want to remove
     */
    public static void removeOrder(Cart cart){
            carthandler.remove(cart);
    }

    /**
     * removes order from list of all orders
     * @param order
     */
    public static void removeOrder(Order order){
        int key = getKeyOfOrder(order);
        if(key!=-1)
            orders.remove(key);
    }

    /**
     * Adds new request to order.
     * @param order order we want to add request to
     * @param goods goods we want to request in order
     * @param number number of goods we want to request in order
     */
    public static void addToOrder(Order order, Goods goods, int number){
        int key = getKeyOfOrder(order);
        if(key==-1){
            addOrder(order);
            key = getKeyOfOrder(order);
        }
        orders.get(key).addToOrder(goods,number);
    }

    /**
     * Gets all carts from system.
     *
     * @return Array of carts in system/null if no cart available
     */
    public static Cart[] getCarts(){
        Set<Cart> set = carthandler.keySet();
        return set != null ? set.toArray(new Cart[0]) : new Cart[0];
    }

    /**
     * Assigns Order to Cart.
     *
     * @param order the order we want to assign
     * @param cart the cart to whom we assign order to
     */
    public static void addOrdertoCart(Order order, Cart cart){
        carthandler.put(cart,order);
    }

    /**
     * Adds cart to system.
     *
     * @param cart cart we want to add
     */
    public static void addCart(Cart cart){
        addOrdertoCart(null, cart);
    }


    /**
     * Moves cart to new coordinates.
     *
     * @param cart cart we want to move
     * @param x x-axis parameter of new position
     * @param y y-axis parameter of new position
     */
    public static void movecart(Cart cart,int x,int y){
        cart.move(x,y);
    }

    /**
     * Gets order assigned to cart.
     *
     * @param cart the cart whose order we want to find
     * @return the order assigned to cart
     */
    public static Order getOrder(Cart cart){
        return carthandler.get(cart);
    }

    /**
     * Gets ID of order
     *
     * @param order the order whose id in system we want  to find
     * @return id(number) of order
     */
    public static int getOrderID(Order order){
        return getKeyOfOrder(order);
    }

    /**
     * Gets the array of goods requested in order.
     *
     * @param order order whose array of goods we want to get
     * @return Array of goods requested in order
     */
    public static Goods[] getOrderGoods(Order order){
        int key = getKeyOfOrder(order);
        if(key==-1){
            addOrder(order);
            key = getKeyOfOrder(order);
        }
        return orders.get(key).getArrGoods();
    }

    /**
     * Loads item to cart.
     *
     * @param cart to which we want to load the item to
     * @param item item we want to load in cart
     */
    public static void loadToCart(Cart cart, Item item){
        cart.addItem(item);
    }

    /**
     * Checks if the order of cart was fully processed(loaded onto cart)
     * @param cart the cart we want to check the order of
     * @return true if order fully loaded
     */
    public static boolean orderprocessed(Cart cart){
        Order order = getOrder(cart);
        for(Goods goods: order.getArrGoods()){
            if(order.getQuantity(goods)!=cart.numofItems(goods)){
                return false;
            }
        }
        return true;
    }

    /**
     * Gets array of goods loaded in cart.
     *
     * @param cart we want to check
     * @return array of goods
     */
    public static Goods[] getGoods(Cart cart){
        return cart.getGoods();
    }

    /**
     * Gets number of items of goods loaded in cart.
     *
     * @param cart the cart we want to check
     * @param goods the goods we want to get the number of
     * @return number of items loaded in cart
     */
    public static int getQuantGoods(Cart cart, Goods goods){
        return cart.numofItems(goods);
    }



}
