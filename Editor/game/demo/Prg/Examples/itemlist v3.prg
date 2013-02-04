class CItem 
{
    private:
        var _ItemName;
        var _ItemQuantity;
        var _ItemDesc;
        var _ItemBuy;
        var _ItemSell;
        
    public:
        method CItem(name, desc, buy, sell);
        method CItem(dummy, itemObj);
        method SetName(newName);
        method GetName();
        method SetQuantity(newAmount);
        method AddQuantity(addAmount);
        method SubQuantity(subAmount);
        method GetQuantity();
        method SetDesc(newDesc);
        method GetDesc();
        method SetBuy(buyAmount);
        method GetBuy();
        method SetSell(sellAmount);
        method GetSell();
    }
    
    class CItemList
    {
    private:
        var _itemInd[];
        var _entryCount;
    public:
        method CItemList();
        method AddEntry(entName, entDesc, entBuy, entSell);
        method RemoveEntry(index);
        method SwapEntry(ind1, ind2);
        method GetEntryTotal();
        method GetEntryName(index);
        method SetEntryName(index, name);
        method GetEntryQuantity(index);
        method SetEntryQuantity(index, amount);
        method AddEntryQuantity(index, amount);
        method SubEntryQuantity(index, amount);
        method GetEntryDesc(index);
        method SetEntryDesc(index, desc);
        method GetEntryBuy(index);
        method SetEntryBuy(index, amount);
        method GetEntrySell(index);
        method SetEntrySell(index, amount);
        method SearchForItem(item);
        method operator[](idx);
    }

method CItem::CItem(name, desc, buy, sell)
{
    _itemName = CastLit(name);
    _itemQuantity = 1
    _itemDesc = CastLit(desc);
    _itemBuy = CastInt(buy);
    _itemSell = CastInt(sell);
}
method CItem::CItem(dummy, itemObj)
{
    this->_itemName = itemObj->_itemName;
    this->_itemQuantity = itemObj->_itemQuantity;
    this->_itemDesc = itemObj->_itemDesc;
    this->_itemBuy = itemObj->_itemBuy;
    this->_itemSell = itemObj->_itemSell;
}
method CItem::SetName(newName)
{
    _itemName = CastLit(newName);
}
method CItem::GetName()
{
    return _itemName;
}
method CItem::SetQuantity(newAmount)
{
    _itemQuantity = CastInt(newAmount);
}
method CItem::AddQuantity(addAmount)
{
    _itemQuantity += addAmount;
}
method CItem::SubQuantity(subAmount)
{
    _itemQuantity -= subAmount;
}
method CItem::GetQuantity()
{
    return _itemQuantity;
}
method CItem::SetDesc(newDesc)
{
    _itemDesc = CastLit(newDesc)
}
method CItem::GetDesc()
{
    return _itemDesc;
}
method CItem::SetBuy(buyAmount)
{
    _itemBuy = CastInt(buyAmount);
}
method CItem::GetBuy()
{
    return _itemBuy;
}
method CItem::SetSell(sellAmount)
{
    _itemSell = CastInt(sellAmount);
}
method CItem::GetSell()
{
    return _itemSell;
}

method CItemList::CItemList()
{
    _entryCount = 0;
}
method CItemList::AddEntry(entName, entDesc, entBuy, entSell)
{
    _entryCount++;
    _itemInd[_entryCount] = CItem(entName, entDesc, entBuy, entSell);
}
method CItemList::RemoveEntry(index)
{
    Local(x);
    Local(y);
    x = index;
    Until(x == _entryCount)
    {
        y = x+1;
        this->SwapEntry(x, y);
        x++;
    }
    _itemInd[x]->release();
    _entryCount--;
}
method CItemList::SwapEntry(ind1, ind2)
{
    tempObj = _itemInd[ind1];
    _itemInd[ind1] = _itemInd[ind2];
    _itemInd[ind2] = tempObj;
    kill(tempObj);
}
method CItemList::GetEntryTotal()
{
    return _entryCount;
}
method CItemList::GetEntryName(index)
{
    return _itemInd[index]->GetName();
}
method CItemList::SetEntryName(index, name)
{
    _itemInd[index]->SetName(name);
}
method CItemList::GetEntryQuantity(index)
{
    return _itemInd[index]->GetQuantity();
}
method CItemList::SetEntryQuantity(index, amount)
{
    if(amount==0)
    {
        this->RemoveEntry(index);
    }
    else
    {
        _itemInd[index]->SetQuantity(amount);
    }
}
method CItemList::AddEntryQuantity(index, amount)
{
    _itemInd[index]->AddQuantity(amount);
}
method CItemList::SubEntryQuantity(index, amount)
{
    _itemInd[index]->SubQuantity(amount);
    if(_itemInd[index]->GetQuantity()<=0)
    {
        this->RemoveEntry(index);
    }
}
method CItemList::GetEntryDesc(index)
{
    return _itemInd[index]->GetDesc();
}
method CItemList::SetEntryDesc(index, desc)
{
    _itemInd[index]->SetDesc(desc);
}
method CItemList::GetEntryBuy(index)
{
    return _itemInd[index]->GetBuy();
}
method CItemList::SetEntryBuy(index, amount)
{
    _itemInd[index]->SetBuy(amount);
}
method CItemList::GetEntrySell(index)
{
    return _itemInd[index]->GetSell();
}
method CItemList::SetEntrySell(index, amount)
{
    _itemInd[index]->SetSell(amount);
}
method CItemList::SearchForItem(item)
{
    Local(x);
    Local(y);
    Local(z);
    z = CastLit(item)
    y = 0;
    For(x=1;x<=_entryCount;x++)
    {
        if(z==_itemInd[x]->GetName())
        {
            y=x;
        }
    }
    return y;
}
method CItemList::operator[](idx)
{
    if(idx>=0)
    {
        return (&obj_item[idx]);
    }
}

