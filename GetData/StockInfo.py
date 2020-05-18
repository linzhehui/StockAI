import datetime as dt
import pandas as pd
import pandas_datareader.data as web
from matplotlib import style
import matplotlib.pyplot as pltimport os 

def GetStockFromYahoo(isHaveStockCode = False):    
if not isHaveStockCode:       
GetHuStock()    
with open('huStock.pickle','rb') as f:        
    tickets = pickle.load(f,encoding='gb2312')    
    if not os.path.exists('StockDir'):        
        os.makedirs('StockDir')            
        for ticket in tickets:        
            arr = ticket.split('(')        
            stock_name = arr[0]        
            stock_code = arr[1][:-1]+'.ss'        
            if os.path.exists('StockDir/{}.csv'.format(stock_name+stock_code)):            
                print('已下载')        
            else:            
                DownloadStock(stock_name,stock_code)            
                print('下载{}中...'.format(stock_name))    
                    
def DownloadStock(stockName,stockCode):    
    style.use('ggplot')    
    start = dt.datetime(2010,1,1)    
    end = dt.datetime(2019,5,10)    #根据股票代码从雅虎财经读取该股票在制定时间段的股票数据    
    df = web.DataReader(stockCode,'yahoo',start,end)    #保存为对应的文件    
    df.to_csv('StockDir/{}.csv'.format(stockName+stockCode))        
    GetStockFromYahoo()
