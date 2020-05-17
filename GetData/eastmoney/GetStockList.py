
import requests

from bs4 import BeautifulSoup

import re

#优化，可以减少程序判断编码所花费的时间

def getHTMLText(url, code='UTF-8'):   
    try:        
         r = requests.get(url)        
         r.raise_for_status()        
         r.encoding = code       
          return r.text   
    except:        
        return "" 

def getStockList(url, stockList):    
    html = getHTMLText(url, 'GB2312')    
    soup = BeautifulSoup(html, 'html.parser')    
    aInformaton = soup.find_all('a')    
    for ainfo in aInformaton:        
        try:            
            stockList.append(re.findall(r'[s][hz]\d{6}', ainfo.attrs['href'])[0])       
        except:            
            continue

def getStockInformation(detailUrl, outputFile, stockList):    
    count = 0    
    for name in stockList:        
        count = count + 1        
        stockUrl = detailUrl + name + '.html'        
        html = getHTMLText(stockUrl)        
        try:            
            if html == "":                
                continue            
            stockDict = {}            
            soup = BeautifulSoup(html, 'html.parser')            
            stockinfo = soup.find('div', attrs={'class': 'stock-bets'})            
            stockname = stockinfo.find('a', attrs={'class': 'bets-name'})            
            # 当标签内部还有标签时，利用text可以得到正确的文字，利用string可能会产生None            
            stockDict["股票名称"] = stockname.text.split()[0]            
            stockKey = stockinfo.find_all('dt')           
            stockValue = stockinfo.find_all('dd')            
            for i in range(len(stockKey)):                
                stockDict[stockKey[i].string] = stockValue[i].string            
                #\r移动到行首，end=""不进行换行           
                print("\r{:5.2f}%".format((count / len(stockList) * 100)), end='')            
                #追加写模式'a'            
                f = open(outputFile, 'a')           
                f.write(str(stockDict) + '\n')  
                f.close()        
        except:
            print("{:5.2f}%".format((count / len(stockList) * 100)), end='')
            continue
        
 def main():    
     listUrl = 'http://quote.eastmoney.com/stocklist.html'    
     detailUrl = 'https://gupiao.baidu.com/stock/'    
     outputFile = 'C:/Users/Administrator/Desktop/out.txt'    
     stockList = []   
     getStockList(listUrl, stockList)    
     getStockInformation(detailUrl, outputFile, stockList)

main()