import time
from selenium import webdriver

browser = webdriver.Chrome('C:/Users/Justin/Desktop/bot/chromedriver')

browser.get("https://www.amazon.com/dp/B08L8LG4M3?smid=ATVPDKIKX0DER&tag=data20-20#aod")

browser.get("https://www.amazon.com/dp/B08J6F174Z?smid=ATVPDKIKX0DER&tag=data20-20#aod")

buyButton = False

while not buyButton:

    try:
        addToCartBtn = addButton - browser.find_element_by_class_name("celwidget")

        print("button isnt ready yet.")

        time.sleep(1)
        browser.refresh()

    except:

        addToCartBtn = addButton - browser.find_element_by_class_name("submit.buy-now")

        print("button was clicked.")
        addToCartBtn.click()
        buyButton = True

        buyit = buyThing - browser.find_element_by_class_name("a-button-input")
        buyit.click()
        print("order was placed")

