package Store.Cart;


import java.util.HashMap;
import java.util.Set;

import Store.StoreData.*;


/**
 * Class that represents the order
 * <p>
 * In stores its Goods, and number of items of this goods the order demands.
 *
 * @author xvenge01 Veronika Vengerova
 */
public class Order {
    HashMap<Goods, Integer> order;

    /**
     * Constructor of empty order
     */
    public Order(){
        this.order = new HashMap<Goods, Integer>();
    }

    /**
     * Gets HasMap representation of order goods and integers
     * @return HasMap representation of order
     */
    public HashMap<Goods, Integer> getOrder(){
        return this.order;
    }

    /**
     * Constructor of order.
     * @param goods Goods the order requests
     * @param numberofgoods Number of Items of this Goods the order requests
     */
    public Order(Goods goods, int numberofgoods){
        this.order = new HashMap<Goods, Integer>();
        this.order.put(goods,numberofgoods);
    }

    /**
     * Gets number of Goods
     * @return number of goods
     */
    public int getNumberofGoods(){
        return this.order.keySet().size();
    }

    /**
     * Gets array of goods in Order
     * @return array of goods in order
     */
    public Goods[] getArrGoods(){
        Set<Goods> set = this.order.keySet();
        return set != null ? set.toArray(new Goods[0]) : new Goods[0];
    }

    /**
     * Gets number of specific goods in order
     * @param goods Goods from order to get the number of
     * @return number of goods in order
     */
    public int getQuantity(Goods goods){
            return this.order.get(goods);
    }

    /**
     * Adds reguest to Order
     * @param goods Type of goods
     * @param numberofgoods Number of this type of Goods
     */
    public void addToOrder(Goods goods, int numberofgoods) {
        if (order.containsKey(goods)) {
            int number = this.order.get(goods);
            this.order.put(goods, number + numberofgoods);
        } else {
            this.order.put(goods, numberofgoods);
        }
    }

    /**
     * Checks if whole Order was processed
     * @param cart Cart that is supposed to process the order
     * @return Whether the whole order was processed
     */
    public boolean checkiffullfilled(Cart cart){
        if(cart.numberofGoods() == getNumberofGoods()){
            Goods[] goods = getArrGoods();
            for(int i =0; i<getNumberofGoods();i++){
                if(cart.numofItems(goods[i]) != order.get(goods[i]).intValue()){
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    /**
     * Removes goods from order
     * @param goods Goods to remove
     * @return Whether removal succesfull
     */
    public boolean removefromOrder(Goods goods){
        if(this.order.remove(goods) == null){
            return false;
        }
        return true;
    }

    /**
     * Reduces number of this type of goods by 1
     * @param goods Type of goods
     */
    public void removeOneGoods(Goods goods){
        if(this.order.containsKey(goods)){/////////////////check if works
            int number = this.order.get(goods);
            if(number > 0){
                this.order.put(goods,number-1);
            }
        }
    }

    /**
     * Gets set of Goods in order
     * @return Set of goods in order
     */
    public Set<Goods> getKeyset(){
        return order.keySet();
    }

}
