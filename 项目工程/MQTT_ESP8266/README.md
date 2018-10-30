# 使用方法

    例如两个模块同时烧入了此程序，则通过串口进行配置，

    首先按照“ssid:password:selfname:targetname?”格式进行配置。

    例如有两个模块分别名为name1和name2。

    wifi名称和密码为ssid和password。

    （此处为了方便两个连接的一个wifi，实际操作时可以是两个不同的wifi）

    则首先打开各自的串口分别发送：

    “ssid:password:name1:name2?”（name1向name2发送数据）、

    “ssid:password:name2:name1?”（name2向name1发送数据）

# 注意

    当串口发送完之后，模块会串口打印出你所配置的wifi名称和密码以及所有的数据
    
    （例如：“ssid:password:name1:name2”）。
    
    你需要进行一次确认是否正确，此配置信息是否正确。
    
    若正确可发送“ok?”进行确认，模块才会尝试去连接wifi和MQTT服务器。
    
    连接成功之后就可以快乐的发送数据啦。
    
    请注意：接收到的信息格式为“where-data”。
    
    例如name1给name2发送了数据data，则name2收到的数据为“name1-data”

    注意确保服务器的MQTT broker正常工作。且此处的服务器连接IP为域名mqtt.shanlong.tech。若更改服务器时，请注意要进行域名解析。

# Arduino环境的安装
    
    1、首先在电脑上获取安装Arduino IDE。
    
    2、然后在“文件→首选项->附加开发板管理器网址”中添加“http://arduino.esp8266.com/stable/package_esp8266com_index.json”（确保已经翻墙）
    
    3、重启IDE
    
    4、在“工具->开发板->开发板管理器”中下拉到最后看见ESP8266，安装最新版
    
    5、在“项目->加载库->添加.zip库”中选择下载的“pubsubclient-master.zip”文件
    
    6、可以开心的编辑了。
    
    7、固件编译与烧写时需要在“工具->开发板”中选择ESP8266的开发板（Generic ESP8266 Module）
    
    8、可以去烧写了
    
    
