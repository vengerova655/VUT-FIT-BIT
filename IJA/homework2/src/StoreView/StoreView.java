package StoreView;

import Store.Cart.Cart;
import Store.Cart.CartHandler;
import Store.Cart.Order;
import Store.StoreObject.*;
import Store.StoreData.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Queue;

/**
 * Class that should act as the View in MVC design.
 * @author xvosci00 Martin Voščinár, xvenge01 Veronika Vengerová
 */
public class StoreView {

    /**
     * Displays list of objects in the given map.
     * @param map map representation which should be displayed
     * @param sizeX horizontal size of the map
     * @param sizeY vertical size of the map
     */
    public void displayObjects(StoreObject[] map, int sizeX, int sizeY) {
        System.out.println("Mapa skladu: ");
        for (int i = 0; i < sizeX; i++) {
            for (int j = 0; j < sizeY; j++) {
                StoreObject obj = map[i + j*sizeX];
                if (obj != null) {
                    System.out.println(objectPosToString(obj, i, j));
                }
            }
        }
    }

    /**
     * Displays the cart (text representation writes parameters).
     * @param cart the cart which we want to represent
     */
    public void displayCart(Cart cart ){
        System.out.println("--VOZIK:"+cart.getName()+"--");
        System.out.println("POLOHA: ["+cart.getX()+","+cart.getY()+"]");
        System.out.println("__obsah__");
        Goods[] goods = cart.getGoods();

        for(Goods good : goods){
            System.out.println(good +": "+ cart.numofItems(good));
        }
        System.out.println("__obsah__");
        System.out.println();
    }

    /**
     * Displays the given object.
     * @param obj object that should be displayed
     */
    public void displayObject(StoreObject obj) {
        System.out.println(objectPosToString(obj));
    }

    /**
     * Displays an object in the given map.
     * @param map map where the object which should be displayed is located
     * @param sizeX horizontal size of the map
     * @param x x - cordinate of the object in the map
     * @param y y - cordinate of the object in the map
     */
    public void displayObject(StoreObject[] map, int sizeX, int x, int y) {
        System.out.println(objectPosToString(map[x + y*sizeX], x, y));
    }

    /**
     * Displays the store map.
     * @param map map representation which should be displayed
     * @param sizeX horizontal size of the map
     * @param sizeY vertical size of the map
     */
    public void displayMap(StoreObject[] map, int sizeX, int sizeY){
        System.out.println(
            "MAPA: (B = uzavretie uličky, S = regál, P = miesto na vzdvihnute)");
        for (int j = 0; j < sizeY; j++) {
            for (int i = 0; i < sizeX;i++) {
                StoreObject object = map[i + j*sizeX];
                if ( object == null) {
                    System.out.print(" - ");
                }
                else if (object.getType() == StoreObject.Type.Blockade) {
                    System.out.print(" B ");
                }
                else if (object.getType() == StoreObject.Type.Shelf) {
                    System.out.print(" S ");
                }
                else if (object.getType() == StoreObject.Type.PickupSpot) {
                    System.out.print(" P ");
                }
            }
            System.out.println();
        }
    }

    /**
     * Creates a String representation of given object position and object info
     * @param obj object, whose String representation should be created
     * @return String representation of given object position and object info
     */
    private String objectPosToString(StoreObject obj) {
        if (obj == null) return "";
        int x = obj.getX();
        int y = obj.getY();
        return objectPosToString(obj, x, y);
    }

    /**
     * Creates a String representation of given object position and object info
     * @param obj object, whose String representation should be created
     * @param x x - coordinate of the object
     * @param y y - coordinate of the object
     * @return String representation of given object position and object info
     */
    private String objectPosToString(StoreObject obj, int x, int y) {
        return "[" + x + "," + y + "] " + obj;
    }

    // public void displayPath() {}

    /**
     * Displays all of the Goods in the store.
     * @param goods array of Goods
     */
    public void displayStoreGoods(Goods[] goods) {
        System.out.println("Tovar na sklade: "+ Arrays.toString(goods));
    }

    /**
     * Displays the given array of Goods.
     * @param goods array of Goods
     */
    public void displayGoods(Goods[] goods) {
        System.out.println(Arrays.toString(goods));
    }

    /**
     * Displays the name and number of Goods in the store.
     * @param goods Goods of which item count is displayed
     * @param count item count which is displayed
     */
    public void displayItemCount(Goods goods, int count) {
        System.out.println("Počet \"" + goods + "\": " + count + " ks");
    }

    /**
     * Displays the name and number of Goods on the given shelf in the store.
     * @param goods Goods of which item count is displayed
     * @param count item count which is displayed
     * @param shelf shelf on which the items are located
     */
    public void displayItemCountOnShelf(Goods goods, int count, Shelf shelf) {
        System.out.println("Počet \""+goods+"\" na " + objectPosToString(shelf)
                            + ": " + count + " ks");
    }

    /**
     * Displays a message saying which items will be added and on which shelf.
     * @param goods Goods of which items will be added
     * @param shelf shelf onto which items will be added
     * @param count number of items that will be added
     */
    public void displayItemsLoad(Goods goods, Shelf shelf, int count) {
        System.out.println("Pridáva sa " + count + " ks \"" + goods
                            + "\" na " + objectPosToString(shelf));
    }

    /**
     * Displays a message saying what object will be added and where.
     * @param obj object which will be added
     * @param x x - coordinate where the object will be added
     * @param y y - coordinate where the object will be added
     */
    public void displayObjectAdd(StoreObject obj, int x, int y) {
        System.out.println("Pridáva sa do mapy: "+objectPosToString(obj, x, y));
    }

    /**
     * Displays a message saying what object will be added and where.
     * @param obj object which will be added
     */
    public void displayObjectAdd(StoreObject obj) {
        System.out.println("Pridáva sa do mapy: " + objectPosToString(obj));
    }

    /**
     * Displays a message saying which items were removed and where from.
     * @param goods Goods of which items were removed
     * @param shelf shelf from which items were removed
     * @param count number of items thet were removed
     */
    public void displayItemRemoval(Goods goods, Shelf shelf, int count) {
        if (shelf == null) {
            System.out.println("Neodstránilo sa nič");
            return;
        }
        System.out.println("Odstránilo sa " + count + " položiek \"" + goods
                            + "\" z " + objectPosToString(shelf)
                          );
    }

    /**
     * Displays the given message String.
     * @param msg String to be displayed
     */
    public void displayMessage(String msg) {
        System.out.println(msg);
    }

    /**
     * Displays the shortest path it found for cart
     * @param queue the queue in which the x and y axis are stored in arrayList
     * @return x and y-axis parameters in ArrayList
     */
    public static ArrayList<Integer>  displaySolution(Queue<ArrayList<Integer>> queue){
        if(queue==null)
            return null;
        ArrayList<Integer> arrint = queue.poll();
        ArrayList<Integer> vysl = arrint;
        while(arrint != null){
            System.out.print(arrint);
            vysl = arrint;
            arrint = queue.poll();
            if(arrint!=null){
                System.out.println("->");
            }
        }
        if(queue.poll()==null) {
            System.out.println();
            return vysl;
        }
        else {
            System.out.println("NO POSSIBLE PATTH");
            return null;
        }
    }

    /**
     * Displays the order text representation
     * @param key the id of order
     * @param order the order
     */
    public void displayOrder(int key,Order order){
//        System.out.println("--------------------");
        System.out.println("ORDER"+key+":");
        for(Goods good : order.getArrGoods()){
            System.out.println(good +": "+ order.getQuantity(good));
        }
        System.out.println("--------------------");

    }
}
