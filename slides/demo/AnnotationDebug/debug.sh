#!/bin/sh

javac -proc:none MyProcessor.java
javac -J-Xdebug -J-Xrunjdwp:transport=dt_socket,server=y,suspend=y,address=5005 -cp . Main.java
