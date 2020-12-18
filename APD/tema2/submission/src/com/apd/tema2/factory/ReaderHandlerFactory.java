package com.apd.tema2.factory;

import com.apd.tema2.Main;
import com.apd.tema2.entities.Car;
import com.apd.tema2.entities.Pedestrians;
import com.apd.tema2.entities.ReaderHandler;
import com.apd.tema2.intersections.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;

/**
 * Returneaza sub forma unor clase anonime implementari pentru metoda de citire din fisier.
 */
public class ReaderHandlerFactory {

    public static ReaderHandler getHandler(String handlerType) {
        // simple semaphore intersection
        // max random N cars roundabout (s time to exit each of them)
        // roundabout with exactly one car from each lane simultaneously
        // roundabout with exactly X cars from each lane simultaneously
        // roundabout with at most X cars from each lane simultaneously
        // entering a road without any priority
        // crosswalk activated on at least a number of people (s time to finish all of them)
        // road in maintenance - 1 lane 2 ways, X cars at a time
        // road in maintenance - N lanes 2 ways, X cars at a time
        // railroad blockage for T seconds for all the cars
        // unmarked intersection
        // cars racing
        return switch (handlerType) {
            case "simple_semaphore" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) {
                    // Exemplu de utilizare:
                    // Main.intersection = IntersectionFactory.getIntersection("simpleIntersection");
                }
            };
            case "simple_n_roundabout" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    String[] line = br.readLine().split(" ");
                    /* How many cars can be in the roundabout at once */
                    SimpleIntersection.max_roundabout_cars = Integer.parseInt(line[0]);
                    /* Time spent by car in roundabout */
                    SimpleIntersection.roundabout_time = Integer.parseInt(line[1]);
                    
                    SimpleIntersection.semaphore = new Semaphore(SimpleIntersection.max_roundabout_cars);
                }
            };
            case "simple_strict_1_car_roundabout" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    String[] line = br.readLine().split(" ");
                    /* Number of directions of roundabout */
                    SimpleIntersection.roundabout_directions = Integer.parseInt(line[0]);
                    /* Time spent by car in roundabout */
                    SimpleIntersection.roundabout_time = Integer.parseInt(line[1]);

                    /* Init barrier. Must wait on each direction */
                    SimpleIntersection.barrier1 = new CyclicBarrier(SimpleIntersection.roundabout_directions);

                    /* Init semaphore. Must wait on 1 car */
                    SimpleIntersection.semaphores = new ArrayList<Semaphore>(SimpleIntersection.roundabout_directions);
                    for (int i = 0; i < SimpleIntersection.roundabout_directions; i++) {
                        Semaphore new_semaphore = new Semaphore(1);
                        SimpleIntersection.semaphores.add(new_semaphore);
                    }
                }
            };
            case "simple_strict_x_car_roundabout" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    String[] line = br.readLine().split(" ");
                    /* Number of directions of roundabout */
                    SimpleIntersection.roundabout_directions = Integer.parseInt(line[0]);
                    /* Time spent by a car in roundabout */
                    SimpleIntersection.roundabout_time = Integer.parseInt(line[1]);
                    /* Cars from one direction at a time */
                    SimpleIntersection.max_car_direction = Integer.parseInt(line[2]);

                    /* Init barriers. Must wait on all cars and also on all cars from all directions*/
                    SimpleIntersection.barrier1 = new CyclicBarrier(Main.carsNo);
                    SimpleIntersection.barrier2 = new CyclicBarrier(SimpleIntersection.roundabout_directions * SimpleIntersection.max_car_direction);

                    /* Init semaphore. Must wait on how many cars are allowed from 1 direction */
                    SimpleIntersection.semaphores = new ArrayList<Semaphore>(SimpleIntersection.roundabout_directions);
                    for (int i = 0; i < SimpleIntersection.roundabout_directions; i++) {
                        Semaphore new_semaphore = new Semaphore(SimpleIntersection.max_car_direction);
                        SimpleIntersection.semaphores.add(new_semaphore);
                    }
                }
            };
            case "simple_max_x_car_roundabout" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    String[] line = br.readLine().split(" ");
                    /* Number of directions of roundabout */
                    SimpleIntersection.roundabout_directions = Integer.parseInt(line[0]);
                    /* Time spent by a car in roundabout */
                    SimpleIntersection.roundabout_time = Integer.parseInt(line[1]);
                    /* Cars from one direction at a time */
                    SimpleIntersection.max_car_direction = Integer.parseInt(line[2]);

                    /* Init barriers. Must wait on all cars from each direction */
                    SimpleIntersection.barrier1 = new CyclicBarrier(SimpleIntersection.max_car_direction);

                    /* Init semaphore. Must wait on how many cars are allowed from 1 direction */
                    SimpleIntersection.semaphores = new ArrayList<Semaphore>(SimpleIntersection.roundabout_directions);
                    for (int i = 0; i < SimpleIntersection.roundabout_directions; i++) {
                        Semaphore new_semaphore = new Semaphore(SimpleIntersection.max_car_direction);
                        SimpleIntersection.semaphores.add(new_semaphore);
                    }
                }
            };
            case "priority_intersection" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    String[] line = br.readLine().split(" ");
                    /* Number of cars with high priority */
                    SimpleIntersection.max_high_priority_cars = Integer.parseInt(line[0]);
                    /* Number of cars with low priority */
                    SimpleIntersection.max_low_priority_cars = Integer.parseInt(line[1]);
                    /* Initiate semaphore */
                    SimpleIntersection.semaphore = new Semaphore(SimpleIntersection.max_high_priority_cars);
                }
            };
            case "crosswalk" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    String[] line = br.readLine().split(" ");
                    /* Time while pedestrians still appear */
                    SimpleIntersection.pedestrian_appearence_time = Integer.parseInt(line[0]);
                    /* Maximum number of pedestrians */
                    SimpleIntersection.max_pedestrians = Integer.parseInt(line[1]);
                    /* Notification monitor */
                    SimpleIntersection.common_notify_monitor = new Object();
                    /* Pedestrians object */
                    Main.pedestrians = new Pedestrians(SimpleIntersection.pedestrian_appearence_time, SimpleIntersection.max_pedestrians);
                }
            };
            case "simple_maintenance" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    String[] line = br.readLine().split(" ");
                    /* Cars waiting to pass the bottleneck */
                    SimpleIntersection.waiting_cars = Integer.parseInt(line[0]);
                    /* Semaphore for direction 0 */
                    SimpleIntersection.semaphore0 = new Semaphore(SimpleIntersection.waiting_cars);
                    /* Semaphore for direction 1 */
                    SimpleIntersection.semaphore1 = new Semaphore(0);
                    /* Notification monitor */
                    SimpleIntersection.common_notify_monitor = new Object();
                    /* Wait for exactly x cars */
                    SimpleIntersection.barrier1 = new CyclicBarrier(SimpleIntersection.waiting_cars);
                }
            };
            case "complex_maintenance" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    
                }
            };
            case "railroad" -> new ReaderHandler() {
                @Override
                public void handle(final String handlerType, final BufferedReader br) throws IOException {
                    /* Barrier so cars wait for the train */
                    SimpleIntersection.barrier1 = new CyclicBarrier(Main.carsNo);
                    /* Queue to keep track of the order of cars */
                    SimpleIntersection.car_queue = new LinkedList<Integer>();
                }
            };
            default -> null;
        };
    }

}
