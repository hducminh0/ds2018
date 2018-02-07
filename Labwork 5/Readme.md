# Working in Hadoop Local (Standalone) Mode

# How to make it works:

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
$ hadoop com.sun.tools.javac.Main LongestPath.java
$ jar cf lp.jar LongestPath*.class
```

2. Assuming that:
* /user/joe/wordcount/input - input directory in HDFS
* /user/joe/wordcount/output - output directory in HDFS

  Make sample text for input:
```
$ bin/hadoop fs -cat /user/joe/longestpath/input/manyline01
this is the first line
sdaadsjdhsklaj  dlwa nkdwl kakl kwm alkda
sample sample sample sample sample sample sample
ok 3-4 line is enough.

$ bin/hadoop fs -cat /user/joe/longestpath/input/manyline02
random random random random random
123812750921401 0 2903 1099 28931098 abcs
2 line in this file
```

  Execute:

```
$ hadoop jar lp.jar LongestPath /user/joe/longestpath/input /user/joe/longestpath/output
```

3. Output:
```
$ cat /user/joe/longestpath/output/part-r-00000
sample sample sample sample sample sample sample  48
```