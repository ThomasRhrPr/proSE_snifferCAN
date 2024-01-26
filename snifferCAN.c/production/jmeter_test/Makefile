# Makefile to generate custom JMeter class
# Created by Hugo BOUY
# Date: 09/06/2023

# Variables definitions
SOURCE_JMETER := /home/thomas/Tools/Test/apache-jmeter-5.5
CUSTOM_CLASS_SOURCE := java/perso
# CLASS_NAME := LengthPrefixedBinaryTCPClientCustom
CLASS_NAME := TCPClientPostmanImpl

all:
	 javac -cp $(SOURCE_JMETER)/lib/ext/ApacheJMeter_core.jar:$(SOURCE_JMETER)/lib/ext/ApacheJMeter_tcp.jar:$(SOURCE_JMETER)/lib/jorphan.jar:$(SOURCE_JMETER)/lib/slf4j-api-1.7.36.jar:$(SOURCE_JMETER)/lib/commons-lang3-3.12.0.jar:$(SOURCE_JMETER)/lib/httpcore-4.4.15.jar:$(SOURCE_JMETER)/lib/httpasyncclient-4.1.5.jar:$(SOURCE_JMETER)/lib/httpclient-4.5.13.jar $(CUSTOM_CLASS_SOURCE)/$(CLASS_NAME).java


clean:
	rm -f $(CUSTOM_CLASS_SOURCE)/$(CLASS_NAME).class

