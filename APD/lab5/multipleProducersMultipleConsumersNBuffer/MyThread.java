package multipleProducersMultipleConsumersNBuffer;
import multipleProducersMultipleConsumersNBuffer.ParBinSearch;

import java.util.concurrent.BrokenBarrierException;

class MyThread extends Thread {
    int id;
    int start;
    int end;
    int aux;

    public MyThread(int id) {
        this.id = id;
    }

    public void run() {
        aux = -1;
        start = (int) (id * (double) ParBinSearch.arr.size() / ParBinSearch.P);
        end = Math.min(ParBinSearch.arr.size() - 1,(int) ((id + 1) * (double) ParBinSearch.arr.size() / ParBinSearch.P));

        if (ParBinSearch.arr.get(ParBinSearch.arrayStart) >= ParBinSearch.el || ParBinSearch.el >= ParBinSearch.arr.get(ParBinSearch.arrayEnd)) {
            ParBinSearch.index = -2;
        }

        while (ParBinSearch.index == -1) {
            if (ParBinSearch.arr.get(start) == ParBinSearch.el)
                aux = start;
            if (ParBinSearch.arr.get(end) == ParBinSearch.el)
                aux = end;
            if (ParBinSearch.arr.get(start) <= ParBinSearch.el && ParBinSearch.arr.get(end) >= ParBinSearch.el) {
                ParBinSearch.arrayStart = start;
                ParBinSearch.arrayEnd = end;
            }
            try {
                ParBinSearch.barrier.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            }
            start = (int) (id * (double) (ParBinSearch.arrayEnd - ParBinSearch.arrayStart) / ParBinSearch.P) + ParBinSearch.arrayStart;
            end = Math.min((int) ((id + 1) * (double) (ParBinSearch.arrayEnd - ParBinSearch.arrayStart) / ParBinSearch.P), ParBinSearch.arrayEnd) + ParBinSearch.arrayStart;
            if (aux != -1)
                ParBinSearch.index = aux;
            try {
                ParBinSearch.barrier.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            }
        }
    }
}