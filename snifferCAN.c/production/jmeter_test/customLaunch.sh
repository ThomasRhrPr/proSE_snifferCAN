#!/bin/bash

# Shell script to launch jmeter with custom class linked
# Created by Hugo BOUY
# Date: 09/06/2023

# SOURCE_JMETER="/Users/hugobouy/Documents/ESEO/E4e_LOCAL/S8_TEST/FRAMEWORK/apache-jmeter-5.5"
SOURCE_JMETER="/home/thomas/Tools/Test/apache-jmeter-5.5"
TEST_PLAN_NAME="Test_Postman.jmx"

$SOURCE_JMETER/bin/jmeter -t $TEST_PLAN_NAME -q user.properties
