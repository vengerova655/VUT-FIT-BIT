package Store;

import Store.StoreObject.*;
import Store.StoreData.*;
import Store.Cart.*;
import StoreController.StoreController;
import StoreView.StoreView;

public class DemonstrationClass {
    static StoreController strcntrl;
    public static void main(String[] args) {
        Store store = new Store(20, 20);
        StoreView strview = new StoreView();

        strcntrl = new StoreController(store, strview);

        Goods goods = new Goods("Horalky");
        Goods goods2 = new Goods("Mila");
        Goods goods3 = new Goods("Treska");

        Shelf shelf = new Shelf(2, 2);
        Shelf shelf4 = new Shelf(2, 12);
        Shelf shelf2 = new Shelf(5, 5);
        Shelf shelf3 = new Shelf(10, 5);




        strcntrl.loadItems(goods, shelf, 5);
        strcntrl.loadItems(goods, shelf4, 6);
        strcntrl.loadItems(goods3, shelf, 5);
        strcntrl.loadItems(goods3, shelf4, 60);
        strcntrl.loadItems(goods, shelf2, 2);
        strcntrl.loadItems(goods2,shelf3,4);
        strcntrl.loadItems(goods,shelf3,20);
        strcntrl.loadItems(goods2,shelf,3);

        System.out.println();
        System.out.println("INVENTORY");
        strcntrl.displayItemCount(goods, shelf);
        strcntrl.displayItemCount(goods2, shelf);
        strcntrl.displayItemCount(goods3, shelf);
        strcntrl.displayItemCount(goods, shelf2);
        strcntrl.displayItemCount(goods2, shelf2);
        strcntrl.displayItemCount(goods3, shelf2);
        strcntrl.displayItemCount(goods, shelf3);
        strcntrl.displayItemCount(goods2, shelf3);
        strcntrl.displayItemCount(goods3, shelf3);
        strcntrl.displayItemCount(goods, shelf4);
        strcntrl.displayItemCount(goods2, shelf4);
        strcntrl.displayItemCount(goods3, shelf4);




        //initializing carts for store
        Cart cart = new Cart(6,9, "IK00124");

        System.out.println();
        System.out.println("Vytvara sa instancia carts, pridavaju sa do systemu");
        strcntrl.displayCart(cart);

        strcntrl.addCart(cart);

        System.out.println("Vytvara sa instancia carts, pridavaju sa do systemu");
        Cart cart2 = new Cart(8,9,"IK00125");
        strcntrl.addCart(cart2);
        strcntrl.displayCart(cart2);

        System.out.println();
        //initializing orders for store

        System.out.println();
        System.out.println("Vytvara sa Instancia orderu, pridavaju sa do systemu");
            Order order = new Order();
            strcntrl.displayOrder(order);
            System.out.println("Pridavam prvok do orderu: " + goods.getName()+ ", "+ 2);
            strcntrl.addToOrder(order, goods3, 2);
            System.out.println("Pridavam prvok do orderu: " + goods2.getName()+  ", "+200);
            strcntrl.addToOrder(order, goods2, 200);
            System.out.println("Pridavam prvok do orderu: " + goods3.getName()+  ", "+30);
            strcntrl.addToOrder(order, goods,30);
            System.out.println("\nOrder s pridanymi prvkami: ");
            strcntrl.displayOrder(order);

            System.out.println("Assigning order1 to cartIK00124");
            strcntrl.addOrder(order,cart);
            System.out.print("Checking if it is loaded in system: ");
            System.out.println(strcntrl.getOrder(cart).equals(order));


            System.out.println();
            System.out.println("Vytvara sa Instancia orderu2, pridavaju sa do systemu");
            Order order2 = new Order();
            strcntrl.displayOrder(order);
            System.out.println("Pridavam prvok do orderu: " + goods3.getName()+ ", "+ 4);
            strcntrl.addToOrder(order2, goods3, 4);
            System.out.println("Pridavam prvok do orderu: " + goods2.getName()+ ", "+ 2);
            strcntrl.addToOrder(order2, goods2, 2);
            System.out.println("Pridavam prvok do orderu: " + goods.getName()+ ", "+ 4);
            strcntrl.addToOrder(order2, goods,3);
            //appointing order to cart

            System.out.println("Assigning order2 to cartIK00125");
            strcntrl.addOrder(order2,cart2);
            System.out.println();
            System.out.print("Checking if it is loaded in system: ");
            System.out.println(strcntrl.getOrder(cart2).equals(order2));






        System.out.println("");
        System.out.println("*************FINDING*PATH*FOR*CART1024*ACCORDING*TO*ASSIGNED*ORDER************");
        processorderdemo(strcntrl, cart, strcntrl.getOrder(cart));
        if(!strcntrl.OrderProcessed(cart))
            System.out.println("WHOLE ORDER WAS NOT PROCESSED...needs future loading");


        System.out.println("******************************************************************************");



        System.out.println("*************FINDING*PATH*FOR*CART1025*ACCORDING*TO*ASSIGNED*ORDER************");
        processorderdemo(strcntrl, cart2, strcntrl.getOrder(cart2));
        if(!strcntrl.OrderProcessed(cart2))
            System.out.println("WHOLE ORDER WAS NOT PROCESSED...needs future loading");


        System.out.println("******************************************************************************");

        System.out.println("INVENTORY CHANGED -> goods are loaded to cart");
        strcntrl.displayItemCount(goods, shelf);
        strcntrl.displayItemCount(goods2, shelf);
        strcntrl.displayItemCount(goods3, shelf);
        strcntrl.displayItemCount(goods, shelf2);
        strcntrl.displayItemCount(goods2, shelf2);
        strcntrl.displayItemCount(goods3, shelf2);
        strcntrl.displayItemCount(goods, shelf3);
        strcntrl.displayItemCount(goods2, shelf3);
        strcntrl.displayItemCount(goods3, shelf3);
        strcntrl.displayItemCount(goods, shelf4);
        strcntrl.displayItemCount(goods2, shelf4);
        strcntrl.displayItemCount(goods3, shelf4);




    }

    public static void processorderdemo(StoreController strcntrl, Cart cart, Order order){
        System.out.println("_________________________");
        System.out.println("CART " + cart.getName() + ", CURRENTLY AT: ["+ cart.getX()+ ", "+ cart.getY() +"]");

        strcntrl.displayClosestPath(cart);

        System.out.println("LOADING AS MUCH AS POSSIBLE FROM THIS SHELF FROM ORDER...");
        System.out.println("WHILE LOADING TO CART, IT IS ALSO REMOVING FROM SHELVES IN INVENTORY LIKE IN REALITY");
        strcntrl.loadMaxToCart(order,cart);
        strcntrl.displayCart(cart);
        System.out.println();

    }
}
