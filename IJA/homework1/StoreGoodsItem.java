package ija.ija2020.homework1.store;

import ija.ija2020.homework1.goods.Goods;
import ija.ija2020.homework1.goods.GoodsItem;

import java.time.LocalDate;


// xvenge01
public class StoreGoodsItem implements GoodsItem {
    public Goods Itemgood;
    public LocalDate inpDate;
    public StoreGoodsItem (Goods good1,LocalDate daten) {
        this.Itemgood = good1;
        this.inpDate = daten;
    }

    @Override
    public Goods goods() {
        return Itemgood;
    }

    @Override
    public boolean sell() {
        return Itemgood.remove(this);
    }
}
