package ija.ija2020.homework1.store;

import ija.ija2020.homework1.goods.Goods;
import ija.ija2020.homework1.goods.GoodsItem;
import ija.ija2020.homework1.goods.GoodsShelf;

import java.util.HashMap;
import java.util.ArrayList;

// xvenge01

public class StoreShelf implements GoodsShelf {

    public HashMap<Goods, ArrayList<GoodsItem>> shelf = new HashMap<>();

    @Override
    public void put(GoodsItem item) {
        if( this.shelf.containsKey( item.goods() ) ){
            this.shelf.get(item.goods()).add(item);
        }
        else {
            ArrayList<GoodsItem> newArr = new ArrayList<>();
            newArr.add(item);
            this.shelf.put(item.goods(),newArr);
        }
    }

    @Override
    public boolean containsGoods(Goods goods) {
        return this.shelf.containsKey(goods);
    }

    @Override
    public GoodsItem removeAny(Goods goods) {
        if(this.containsGoods(goods)) {
            return this.shelf.get(goods).remove(0);
        }
        return null;
    }

    @Override
    public int size(Goods goods) {
        if(this.containsGoods(goods)) {
            return this.shelf.get(goods).size();
        }
        return 0;
    }
}
