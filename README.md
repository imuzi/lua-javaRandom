# lua-javaRandom
reference: Java.Util.Random package



    local randomJava = require("randomJava")
    randomJava.setSeed(1520)


    for i=1,100 do

     
      print("randomJava.nextInt(100)",randomJava.nextInt(100))
      
      
    end
