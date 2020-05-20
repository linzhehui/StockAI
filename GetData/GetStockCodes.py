import bs4 as bs
import requests #python的http客户端
import pickle #用于序列化反序列化

def GetHuStock():    
    result= [] 
    res = requests.get('https://www.banban.cn/gupiao/list_sh.html')    #防止中文乱码    
    res.encoding = res.apparent_encoding    #使用bsoup的lxml样式    
    soup = bs.BeautifulSoup(res.text,'lxml')    #从html内容中找到类名为'u-postcontent cz'的div标签    
    content = soup.find('div',{'class':'u-postcontent cz'})    
  
    #fo = open("Stocklist.txt", "w")
    for item in content.findAll('a'):        
        result.append(item.text)   
        #fo.write('\n')
        #fo.write(item.text)
    #fo.close()

    res = requests.get('https://www.banban.cn/gupiao/list_sz.html')    #防止中文乱码    
    res.encoding = res.apparent_encoding    #使用bsoup的lxml样式    
    soup = bs.BeautifulSoup(res.text,'lxml')    #从html内容中找到类名为'u-postcontent cz'的div标签    
    content = soup.find('div',{'class':'u-postcontent cz'})  
    for item in content.findAll('a'):        
        result.append(item.text)   

    res = requests.get('https://www.banban.cn/gupiao/list_cyb.html')    #防止中文乱码    
    res.encoding = res.apparent_encoding    #使用bsoup的lxml样式    
    soup = bs.BeautifulSoup(res.text,'lxml')    #从html内容中找到类名为'u-postcontent cz'的div标签    
    content = soup.find('div',{'class':'u-postcontent cz'})  
    for item in content.findAll('a'):        
        result.append(item.text)   


    with open('huStock.pickle','wb') as f:        
        pickle.dump(result,f)

def GetStockList(url, filename):    
    res = requests.get(url)    #防止中文乱码    
    res.encoding = res.apparent_encoding    #使用bsoup的lxml样式    
    soup = bs.BeautifulSoup(res.text,'lxml')    #从html内容中找到类名为'u-postcontent cz'的div标签    
    content = soup.find('div',{'class':'u-postcontent cz'})    
    fo = open(filename, "w")
    for item in content.findAll('a'):         
        fo.write('\n')
        fo.write(item.text)
    fo.close()



GetHuStock()
GetStockList('https://www.banban.cn/gupiao/list_sz.html', 'szStockList.txt')
GetStockList('https://www.banban.cn/gupiao/list_sh.html', 'shStockList.txt')
GetStockList('https://www.banban.cn/gupiao/list_cyb.html', 'cybStockList.txt')