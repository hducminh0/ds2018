# Work in Hadoop Local (Standalone) Mode

# How to make it work:

## Requirement:

_Hadoop: Setting up a Single Node Cluster_

1. Download stable version of Hadoop, put it in somewhere safe !
2. Set the JAVA_HOME and Hadoop environment
```bash
	export JAVA_HOME= <your-java-home-path>
	export HADOOP_CLASSPATH="${JAVA_HOME}/lib/tools.jar"
	export HDP_HOME= <your-hadoop-folder>
	export HDP=$HDP_HOME/bin
	export PATH=$HDP:$PATH
```
3. Cd to hadoop folder. Copy config to input folder.
```
$ mkdir input
$ cp etc/hadoop/*.xml input
```
4. try `hadoop` command. It should look like this:
```
Usage: hadoop [--config confdir] [COMMAND | CLASSNAME]
  CLASSNAME            run the class named CLASSNAME
 or
  where COMMAND is one of:
  fs                   run a generic filesystem user client
  version              print the version
  jar <jar>            run a jar file
                       note: please use "yarn jar" to launch
                             YARN applications, not this command.
  checknative [-a|-h]  check native hadoop and compression libraries availability
  distcp <srcurl> <desturl> copy file or directories recursively
  archive -archiveName NAME -p <parent path> <src>* <dest> create a hadoop archive
  classpath            prints the class path needed to get the
                       Hadoop jar and the required libraries
  credential           interact with credential providers
  daemonlog            get/set the log level for each daemon
  trace                view and modify Hadoop tracing settings

Most commands print help when invoked w/o parameters.
```
5. Done Local Install. 
   More: https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/SingleCluster.html

## Compile and Execute:

1.Compile WordCount.java and create a jar:
``` 
$ hadoop com.sun.tools.javac.Main WordCount.java
$ jar cf wc.jar WordCount*.class
```

2. Assuming that:
* /user/joe/wordcount/input - input directory in HDFS
* /user/joe/wordcount/output - output directory in HDFS

  	Make sample text for input:
```
$ bin/hadoop fs -cat /user/joe/wordcount/input/file01
Hello World Bye World

$ bin/hadoop fs -cat /user/joe/wordcount/input/file02
Hello Hadoop Goodbye Hadoop
```
	Execute:
```
$ hadoop jar wc.jar WordCount /user/joe/wordcount/input /user/joe/wordcount/output
```

3. Output:
```
$ cat /user/joe/wordcount/output/part-r-00000
Bye 1
Goodbye 1
Hadoop 2
Hello 2
World 2
```