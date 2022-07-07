package ija.ija2020.homework1.store;

import ija.ija2020.homework1.goods.Goods;
import ija.ija2020.homework1.goods.GoodsItem;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Objects;

// xvenge01
public class StoreGoods implements Goods {
    public String nameofgoods;
    public ArrayList <GoodsItem> zoznam = new ArrayList<>();
    public StoreGoods (String x) {
        this.nameofgoods = x;

    }

    @Override
    public String getName() {
        return this.nameofgoods;
    }

    @Override
    public boolean addItem(GoodsItem goodsItem) {
        return this.zoznam.add(goodsItem);
    }

    @Override
    public GoodsItem newItem(LocalDate localDate) {

        GoodsItem obj = new StoreGoodsItem(this,localDate);
        zoznam.add(obj);
        return obj;
    }

    @Override
    public boolean remove(GoodsItem goodsItem) {
        return this.zoznam.remove(goodsItem);
    }

    @Override
    public boolean empty() {
        return this.zoznam.size() == 0;
    }

    @Override
    public int size() {
        return this.zoznam.size();
    }


    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof StoreGoods)) return false;
        StoreGoods that = (StoreGoods) o;
        return this.nameofgoods.equals(that.nameofgoods);
    }

    @Override
    public int hashCode() {
        return Objects.hash(nameofgoods);
    }
}
