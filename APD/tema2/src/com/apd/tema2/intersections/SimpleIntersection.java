package com.apd.tema2.intersections;

import java.util.ArrayList;
import java.util.Deque;
import java.util.Queue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;

import com.apd.tema2.entities.Car;
import com.apd.tema2.entities.Intersection;

public class SimpleIntersection implements Intersection {
    // Define your variables here.
    
    /* For second task */
    public static int max_roundabout_cars;
    public static int roundabout_time;
    public static Semaphore semaphore;
    
    /* For third task */
    public static int roundabout_directions;
    public static CyclicBarrier barrier1;
    public static CyclicBarrier barrier2;
    public static ArrayList <Semaphore> semaphores;
    
    /* For fourth task  and fifth task */
    public static int max_car_direction;

    /* For sixth task */
    public static int max_high_priority_cars;
    public static int max_low_priority_cars;

    /* For seventh task */
    public static int pedestrian_appearence_time;
    public static int max_pedestrians;
    public static Object common_notify_monitor;
    public static String current_color;

    /* For eith task */
    public static int waiting_cars;
    public static Semaphore semaphore0;
    public static Semaphore semaphore1;

    /* For tenth task */
    public static Deque<Integer> car_queue;
}
