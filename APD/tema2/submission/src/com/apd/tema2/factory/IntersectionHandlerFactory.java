package com.apd.tema2.factory;

import com.apd.tema2.Main;
import com.apd.tema2.entities.*;
import com.apd.tema2.intersections.*;
import com.apd.tema2.utils.Constants;

import static java.lang.Thread.sleep;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

/**
 * Clasa Factory ce returneaza implementari ale InterfaceHandler sub forma unor
 * clase anonime.
 */
public class IntersectionHandlerFactory {

    public static IntersectionHandler getHandler(String handlerType) {
        // simple semaphore intersection
        // max random N cars roundabout (s time to exit each of them)
        // roundabout with exactly one car from each lane simultaneously
        // roundabout with exactly X cars from each lane simultaneously
        // roundabout with at most X cars from each lane simultaneously
        // entering a road without any priority
        // crosswalk activated on at least a number of people (s time to finish all of
        // them)
        // road in maintenance - 2 ways 1 lane each, X cars at a time
        // road in maintenance - 1 way, M out of N lanes are blocked, X cars at a time
        // railroad blockage for s seconds for all the cars
        // unmarked intersection
        // cars racing
        return switch (handlerType) {
            case "simple_semaphore" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                    /* Car has reached the roundabout */
                    System.out.println("Car " + car.getId() + " has reached the semaphore, now waiting...");

                    /* Car is going trough the roundabout */
                    try {
                        sleep(car.getWaitingTime());
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    /* Car has exited the roundabout */
                    System.out.println("Car " + car.getId() + " has waited enough, now driving...");
                }
            };
            case "simple_n_roundabout" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                    /* Car has reached the roundabout */
                    System.out.println("Car " + car.getId() + " has reached the roundabout, now waiting...");

                    /* Car has entered the roundabout and the semaphore has turned red */
                    try {
                        SimpleIntersection.semaphore.acquire();
                        System.out.println("Car " + car.getId() + " has entered the roundabout");
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    /* Car is going trough the roundabout */
                    try {
                        sleep(SimpleIntersection.roundabout_time);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    /* Car has exited the roundabout and the semaphore turned green */
                    System.out.println("Car " + car.getId() + " has exited the roundabout after "
                            + SimpleIntersection.roundabout_time / 1000 + " seconds");

                    /* Semaphore turns green */
                    SimpleIntersection.semaphore.release();
                }
            };
            case "simple_strict_1_car_roundabout" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                    /* Car has reached the roundabout */
                    System.out.println("Car " + car.getId() + " has reached the roundabout");

                    /* The semaphore turned red */
                    try {
                        SimpleIntersection.semaphores.get(car.getStartDirection()).acquire();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    /* Car has entered the roundabout */
                    System.out.println(
                            "Car " + car.getId() + " has entered the roundabout from lane " + car.getStartDirection());

                    /* Car is going trough the roundabout */
                    try {
                        sleep(SimpleIntersection.roundabout_time);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    /* Car has exited the roundabout */
                    System.out.println("Car " + car.getId() + " has exited the roundabout after "
                            + SimpleIntersection.roundabout_time / 1000 + " seconds");

                    /* Car is waiting for all other cars to exit the roundabout */
                    try {
                        SimpleIntersection.barrier1.await();
                    } catch (BrokenBarrierException | InterruptedException e) {
                        e.printStackTrace();
                    }

                    /* Semaphore turns green */
                    SimpleIntersection.semaphores.get(car.getStartDirection()).release();

                }
            };
            case "simple_strict_x_car_roundabout" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                    System.out.println("Car " + car.getId() + " has reached the roundabout, now waiting...");

                    /* Wait for all other cars to reach the roundabout */
                    try {
                        SimpleIntersection.barrier1.await();
                    } catch (InterruptedException | BrokenBarrierException e) {
                        e.printStackTrace();
                    }

                    /* Select car to enter the roundabout. Semaphore turns red afret x selections */
                    try {
                        SimpleIntersection.semaphores.get(car.getStartDirection()).acquire();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    System.out.println("Car " + car.getId() + " was selected to enter the roundabout from lane "
                            + car.getStartDirection());

                    /* Wait for all selections to be made */
                    try {
                        SimpleIntersection.barrier2.await();
                    } catch (BrokenBarrierException | InterruptedException e) {
                        e.printStackTrace();
                    }

                    /* Car enters the roundabout */
                    System.out.println(
                            "Car " + car.getId() + " has entered the roundabout from lane " + car.getStartDirection());

                    /* Car is going trough the roundabout */
                    try {
                        sleep(SimpleIntersection.roundabout_time);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    /* Wait for all car to reach the exit */
                    try {
                        SimpleIntersection.barrier2.await();
                    } catch (InterruptedException | BrokenBarrierException e) {
                        e.printStackTrace();
                    }

                    /* Car is waiting for all other cars to exit the roundabout */
                    System.out.println("Car " + car.getId() + " has exited the roundabout after "
                            + SimpleIntersection.roundabout_time / 1000 + " seconds");

                    /* The semaphore turns green */
                    SimpleIntersection.semaphores.get(car.getStartDirection()).release();
                }
            };
            case "simple_max_x_car_roundabout" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                    // Get your Intersection instance

                    try {
                        sleep(car.getWaitingTime());
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    } // NU MODIFICATI

                    /* Car reached the roundabout */
                    System.out.println(
                            "Car " + car.getId() + " has reached the roundabout from lane " + car.getStartDirection());

                    /* Select car to enter the roundabout. Semaphore turns red afret x selections */
                    try {
                        SimpleIntersection.semaphores.get(car.getStartDirection()).acquire();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    /* Car enters the roundabout */
                    System.out.println(
                            "Car " + car.getId() + " has entered the roundabout from lane " + car.getStartDirection());

                    /* Car is going trough the roundabout */
                    try {
                        sleep(SimpleIntersection.roundabout_time);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    /* Wait for all cars to reach the exit */
                    try {
                        SimpleIntersection.barrier1.await();
                    } catch (InterruptedException | BrokenBarrierException e) {
                        e.printStackTrace();
                    }

                    /* Car is waiting for all other cars to exit the roundabout */
                    System.out.println("Car " + car.getId() + " has exited the roundabout after "
                            + SimpleIntersection.roundabout_time / 1000 + " seconds");

                    /* The semaphore turns green */
                    SimpleIntersection.semaphores.get(car.getStartDirection()).release();
                }
            };
            case "priority_intersection" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                    // Get your Intersection instance
                    try {
                        sleep(car.getWaitingTime());
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    } // NU MODIFICATI

                    /* Masinile cu prioritate mica */
                    if (car.getPriority() == 1) {
                        System.out.println(
                                "Car " + car.getId() + " with low priority is trying to enter the intersection...");
                        /* Get all tokens so no other low priority car can enter */
                        try {
                            SimpleIntersection.semaphore.acquire(SimpleIntersection.max_high_priority_cars);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }

                        /* Car entered the intersection */
                        System.out.println("Car " + car.getId() + " with low priority has entered the intersection");

                        /* Car exited the intersection */
                        SimpleIntersection.semaphore.release(SimpleIntersection.max_high_priority_cars);
                    }
                    /* Masinile cu prioritate mare */
                    else {
                        /* Get instant access to the intersection */
                        try {
                            SimpleIntersection.semaphore.acquire(1);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }

                        /* Car entered the intersection */
                        System.out.println("Car " + car.getId() + " with high priority has entered the intersection");

                        /* Go trough intersection */
                        try {
                            sleep(2000);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }

                        /* Car exits intersection */
                        System.out.println("Car " + car.getId() + " with high priority has exited the intersection");
                        SimpleIntersection.semaphore.release();
                    }
                }
            };
            case "crosswalk" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {

                    boolean was_green = false;

                    while (!Main.pedestrians.isFinished()) {
                        /* Needs mutex because it will be accessed from diffrent threads */
                        synchronized (SimpleIntersection.common_notify_monitor) {
                            /* Pedestrians pass */
                            while (Main.pedestrians.isPass()) {
                                /* Check not to duplicate message */
                                if (was_green) {
                                    System.out.println("Car " + car.getId() + " has now red light");
                                    was_green = false;
                                }
                                try {
                                    SimpleIntersection.common_notify_monitor.wait();
                                } catch (InterruptedException e) {
                                    e.printStackTrace();
                                }
                            }

                            if (!was_green) {
                                System.out.println("Car " + car.getId() + " has now green light");
                                was_green = true;
                            }
                        }
                    }
                }
            };
            case "simple_maintenance" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {

                    /* When cars come from direction 0 */
                    if (car.getStartDirection() == 0) {
                        /* Car has reached */
                        synchronized (SimpleIntersection.common_notify_monitor) {
                            System.out.println("Car " + car.getId() + " from side number " + car.getStartDirection()
                                    + " has reached the bottleneck");
                        }

                        /* Decrease semaphore of own lane */
                        try {
                            SimpleIntersection.semaphore0.acquire();
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }

                        /* If is the last car reset the semaphore on other lane and on current lane */
                        if (SimpleIntersection.semaphore0.availablePermits() == 0) {
                            SimpleIntersection.semaphore1.release(SimpleIntersection.waiting_cars);
                        }

                        synchronized(SimpleIntersection.common_notify_monitor) {
                            System.out.println("Car " + car.getId() + " from side number "
                            + car.getStartDirection() + " has passed the bottleneck");
                        }
                    }
                    /* When cars come from direction 1. Mirror lane 0 */
                    else {

                            synchronized(SimpleIntersection.common_notify_monitor) {
                                System.out.println("Car " + car.getId() + " from side number " + car.getStartDirection()
                                + " has reached the bottleneck");
                            }
                            try {
                                SimpleIntersection.semaphore1.acquire();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }

                            if (SimpleIntersection.semaphore1.availablePermits() == 0) {

                                SimpleIntersection.semaphore0.release(SimpleIntersection.waiting_cars);
                            }

                            synchronized(SimpleIntersection.common_notify_monitor) {
                                System.out.println("Car " + car.getId() + " from side number "
                                + car.getStartDirection() + " has passed the bottleneck");
                            }
                    }
                }
            };
            case "complex_maintenance" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {

                }
            };
            case "railroad" -> new IntersectionHandler() {
                @Override
                public void handle(Car car) {
                    /* Car gets queued for crossing the railroad and waits */
                    synchronized (SimpleIntersection.car_queue) {
                        System.out.println("Car " + car.getId() + " from side number " + car.getStartDirection()
                            + " has stopped by the railroad");
                        SimpleIntersection.car_queue.addLast(car.getId());
                    }

                    /* Wait for message to be printed */
                    try {
                        SimpleIntersection.barrier1.await();
                    } catch (InterruptedException | BrokenBarrierException e) {
                        e.printStackTrace();
                    }

                    /* The train passes after all cars get queued. Only the first car in queue
                     * will print the message 
                     */
                    synchronized(SimpleIntersection.car_queue) {
                        if (car.getId() == SimpleIntersection.car_queue.peek()) {
                            System.out.println("The train has passed, cars can now proceed");
                        }
                    }

                    /* The other threads must wait for this message to be printed */
                    try {
                        SimpleIntersection.barrier1.await();
                    } catch (InterruptedException | BrokenBarrierException e) {
                        e.printStackTrace();
                    }

                    /* The car waits to be the first in queue in order to get dequed */
                    synchronized(SimpleIntersection.car_queue) {
                        while (car.getId() != SimpleIntersection.car_queue.peek()) {
                            try {
                                SimpleIntersection.car_queue.wait();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }
                    }

                    /* When car is done waiting, get dequeued */
                    synchronized(SimpleIntersection.car_queue) {
                        System.out.println("Car " + car.getId() + " from side number " + car.getStartDirection()
                            + " has started driving");
                        SimpleIntersection.car_queue.removeFirst();
                        SimpleIntersection.car_queue.notifyAll();
                    }
                }
            };
            default -> null;
        };
    }
}
