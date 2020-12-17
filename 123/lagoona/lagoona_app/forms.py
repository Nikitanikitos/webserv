from django import forms


class BookingForm(forms.Form):
	city = forms.CharField(
		label='',
		max_length=50,
		widget=forms.TextInput(attrs={'class': 'booking-input input', 'placeholder': 'Город'}))
	room_category = forms.CharField(
		label='',
		max_length=50,
		widget=forms.TextInput(attrs={'class': 'booking-input input', 'placeholder': 'Категория номера'}))
	check_in_date = forms.DateField(
		label='',
		widget=forms.TextInput(attrs={'class': 'booking-input input', 'placeholder': 'Дата заселения'}))
	count_of_nights = forms.IntegerField(
		label='',
		min_value=1, widget=forms.TextInput(attrs={'class': 'booking-input input', 'placeholder': 'Колличество ночей'}))
	count_of_guests = forms.IntegerField(
		label='',
		min_value=1, widget=forms.TextInput(attrs={'class': 'booking-input input', 'placeholder': 'Колличество гостей'}))


class WriteToUsForm(forms.Form):
	name = forms.CharField(
		label='',
		max_length=50,
		widget=forms.TextInput(attrs={'class': 'input write-to-us-input', 'placeholder': 'Имя'}))
	number = forms.CharField(
		label='',
		max_length=50,
		widget=forms.TextInput(attrs={'class': 'input write-to-us-input', 'placeholder': '+7'}))
	content = forms.CharField(
		label='',
		max_length=200,
		widget=forms.Textarea(attrs={'class': 'input write-to-us-input', 'placeholder': 'Сообщение...'}))