#CalculatorServer.java

package server;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.*;
import org.omg.PortableServer.*;

import calculator_module.Calculator;
import calculator_module.CalculatorHelper;

public class CalculatorServer{
    public static void main(String[] args){
        try{
            ORB orb=ORB.init(args,null);
            POA rootpoa=(POA)orb.resolve_initial_references("RootPOA");
            rootpoa.the_POAManager().activate();

            CalculatorImpl calculatorImpl=new CalculatorImpl();
            calculatorImpl.setORB(orb);

            org.omg.CORBA.Object ref=rootpoa.servant_to_refernece(calculatorImpl);
            Calculator href=CalculatorHelper.narrow(ref);

            org.omg.CORBA.Object objRef=orb.resolve_initial_references("NameService");
            NamingContextExt ncRef=NamingContextExtHelper.narrow(objRef);

            String name="Calculator";
            NameComponent path[]=ncRef.to_name(name);
            ncRef.rebind(path,href);

            System.out.println("CAlculator server ready and waiting...");
            orb.run();
        }
        catch(Exception e){
            System.err.println("Error: "+e);
            e.printStackTrace(System.out);
        }
        finally{
            System.out.println("Calcuatorserver exiting ...");
        }
    }
}

#CalculatorClient.java---->

package client;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.*;
import org.omg.CORBA.ORBPackage.InvalidName;

import calculator_module.Calculator;
import calculator_module.CalculatorHelper;

public class CalculatorClient {
    public static void main(String[] args) {
        try {
            ORB orb = ORB.init(args, null);

            org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
            NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

            String name = "Calculator";
            Calculator calculator = CalculatorHelper.narroe(ncRef.resolve_str(name));

            System.out.println("Obtained a handle on server object");
            System.out.println(calculator.add(1, 2));
            System.out.println(calculator.subtract(4, 2));
        } catch (Exception e) {
            System.err.println("Error:" + e);
            e.printStackTrace(System.out);
        }
    }}

    idlj-fall calculator.idl folders-->calculator_module server-->CalculatorImpl.java CalculatorServer.java

    client-->CalculatorServer.java

module calculator_module{

    interface Calculator {
        long add(in long a,in long b);
        oneway

        void shutdown();
    };
};

javac./*/*.java
orbd -ORBInitialPort 1050 -ORBInitialHost localhost

java server.CalculatorServer -ORBInitialPort 1050 -ORBInitialHost localhost
java client.CalculatorClient -ORBInitialProt 1050 -ORBInitialHost localhost

