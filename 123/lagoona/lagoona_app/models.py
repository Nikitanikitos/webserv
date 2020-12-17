from django.db import models


class Hotel(models.Model):
	image_url = models.CharField(max_length=200)
	room_price = models.IntegerField()
	hotel_name = models.CharField(max_length=200)
	hotel_location = models.CharField(max_length=200)
	rating = models.IntegerField(max_length=5)


class HeaderMenu(models.Model):
	item_menu = models.CharField(max_length=50)
	link_to = models.CharField(max_length=50)
