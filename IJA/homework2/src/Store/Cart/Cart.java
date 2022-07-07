package Store.Cart;

import Store.StoreData.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Set;


/**
 * @author xvenge01 Veronika Vengerova
 */
public class Cart {
    private String name;
    private int maximumweight=50;
    private int currentval = 0;
    private int x;
    private int y;
    private HashMap<Goods, ArrayList<Item>> items = new HashMap<Goods, ArrayList<Item>>();

    /**
     * Gets goods already loaded in cart.
     * @return Array of goods in cart
     */
    public Goods[] getGoods(){
        Set<Goods> set = this.items.keySet();
        return set != null ? set.toArray(new Goods[0]) : new Goods[0];
    }

    /**
     * Gets the name of the cart
     * @return The name of the Cart
     */
    public String getName(){
        return this.name;
    }

    /**
     * Gets number of goods in cart
     * @return int, number of Goods in cart
     */
    public int numberofGoods(){
        if(items.keySet()!=null)
            return items.keySet().size();
        return 0;
    }

    /**
     * Gets the x-axis parameter of where the cart is located at.
     * @return x-axis location
     */
    public int getX(){
        return this.x;
    }

    /**
     * Gets the y-axis parameter of where the cart is located at.
     * @return x-axis location
     */
    public int getY(){
        return this.y;
    }

    /**
     * Adds Item to cart
     * @param item Item to add to cart
     * @return Successfully added to cart
     */
    public boolean addItem(Item item) {
        Goods goods = item.getGoods();
        ArrayList<Item> list = this.items.get(goods);

        if (list == null) {
            this.items.put(goods, new ArrayList<Item>());
            list = this.items.get(goods);
        }

        if(list.add(item)){
            currentval+=1;
            return true;
        }
        return false;
    }


    /**
     * Gets current weight(number of items loaded in cart)
     * @return the weight of cart
     */
    public int getCurrentWeight(){
        return this.currentval;
    }

    /**
     * Removes goods from cart
     * @param goods goods to remove
     * @return item that was removed from the cart
     */
    public Item removeItem(Goods goods) {
        ArrayList<Item> list = this.items.get(goods);
        return (list != null && !list.isEmpty()) ? list.remove(0) : null;
    }


    /**
     * Checks whether the cart is fully loaded
     * @return true if the cart is fully loaded
     */
    public boolean isFull(){
        return currentval==maximumweight;
    }

    /**
     * Gets number of Items of type Goods
     * @param goods Type of goods we want the number of
     * @return int, number of items of type Goods
     */
    public int numofItems(Goods goods){
        if (items.get(goods)!=null){
            return items.get(goods).size();
        }
        return 0;
    }

    /**
     * Constructor of Cart.
     * <p>
     * We need x,y-axis location of cart and name
     *
     * @param startingx x-axis parameter of startingpoint
     * @param startingy y-axis parameter of startingpoint
     * @param name name of the cart
     */
    public Cart(int startingx, int startingy, String name){
        this.name = name;
        this.x = startingx;
        this.y = startingy;
    }

    /**
     * Constructor of Cart without name.
     * @param startingx x-axis parameter of startingpoint
     * @param startingy y-axis parameter of startingpoint
     */
    public Cart(int startingx, int startingy){
        this.x = startingx;
        this.y = startingy;
    }

    /**
     * Moves cart to new position
     * @param i x-axis posiiton
     * @param j y-axis position
     */
    public void move(int i, int j){
        this.x =i;
        this.y = j;
    }


}
