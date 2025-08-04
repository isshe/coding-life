import easyquotation

quotation = easyquotation.use('qq')

# quotation.market_snapshot(prefix=True))
print(quotation.real(['sh000001', 'sz000001'], prefix=True))
