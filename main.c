#include <stdio.h>
#include <omp.h>
#define N 100
#define NUM_PROCESSORS 4

int main()
{
    int arr[N];
    for (int i = 0; i < N; i++)
    {
        arr[i] = sizeof(int) * i;
    }

    int sum = 0;
    int PARTIAL_SUM[NUM_PROCESSORS];

#pragma omp parallel num_threads(NUM_PROCESSORS)
    {
        int thread_id = omp_get_thread_num();
        int start = thread_id * (N / NUM_PROCESSORS);
        int end = (thread_id + 1) * (N / NUM_PROCESSORS);
        PARTIAL_SUM[thread_id] = 0;
        for (int i = start; i < end; i++)
        {
            PARTIAL_SUM[thread_id] += arr[i];
        }
    }

    for (int i = 0; i < NUM_PROCESSORS; i++)
    {
        sum += PARTIAL_SUM[i];
        printf("Partial sum of thread %d : %d\n", i, PARTIAL_SUM[i]);
    }

    printf("Sum : %d\n", sum);
    return 0;
}

// to compile : gcc -fopenmp main.c -o output
// for output : ./output

/*
three folders: client remotes server
in client package make file : ClientRequest.java
in server package make file : SearchServer.java
in remotes package make files: Search.java and SerachQuery.java

Search.java ---->

package remotes;
import java.rmi.Remote;
import java.rmi.RemoteException;
public interface Search extends Remote{
    public String query(String search) throws RemoteException;
}

SearchQuery.java ---->

package remotes;
import java.rmi.*;
import java.rmi.server.*;

public class SearchQuery extends UnicastRemoteObject implements Search{
    public SearchQuery() throws RemoteException{
        super();
    }
    public String query(String search) throws RemoteException{
        String result="No result found!";
        if(search.equals("p2p")){
            result="Found 1 result";
        }
        return result;
    }
}

SearchServer.java ---->

package server;
import java.rmi.*;
import java.rmi.registry.*;
import remotes.Search;
import remotes.SearchQuery;

public class SearchServer{
    public static void main(String[] args){
        try{
            Search search=new SearchQuery();
            Registry registry=LoacteRegistry.createRegistry(1099);
            Naming.rebind("rmi://localhost:1099"+"/REMOTE_SERACH",search);
            System.out.println("Search Server Ready...");
        }catch(Exception e){
            System.out.println("Server issue: "+e.getMessage());
        }

    }

}

ClientRequest.java ---->

package client;
import java.rmi.*;
import remotes.Search;

public class ClientRequest{
    public static void main(String[] args){
        try{
            String search=(args.length<1)?"p2p":args[0];
            String url="rmi://localhost:1099/REMOTE_SEARCH"
            Search access=(Search)Naming.lookup(url);
            String result=access.query(search);
            Sytem.out.println("Found : "+result);
        }catch(Exception e){
            System.out.println("Client Request Exception : "+e.getMessage());
        }
    }
}

To compile: javac ./*/
    *.java
        on one terminal : java server /
    SearchServer.java
        on other terminal : java client /
    ClientRequest.java

        * /