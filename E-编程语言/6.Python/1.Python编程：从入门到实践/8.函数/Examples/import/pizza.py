# -*- coding: utf-8 -*

def make_pizza(size, *toppings):
    """概述要制作的比萨"""
    print("\nMaking a " + str(size) +
        "-inch pizza with the following toppings:")
    for topping in toppings:
        print("- " + topping)

def make_pizza2(size, *toppings):
    make_pizza(size, *toppings)
