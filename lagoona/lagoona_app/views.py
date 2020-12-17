from django.shortcuts import render
from django.views.generic.base import View
from .forms import BookingForm, WriteToUsForm
from .models import HeaderMenu, Hotel


class LagoonaView(View):

	def get(self, request):
		booking_form = BookingForm()
		write_us_form = WriteToUsForm()
		list_menu = HeaderMenu.objects.all()
		hotel_list = Hotel.objects.all()
		return render(
			request, 'index.html', context={
				'booking_form': booking_form,
				'list_menu': list_menu,
				'write_us_form': write_us_form,
				'hotel_list': hotel_list})
