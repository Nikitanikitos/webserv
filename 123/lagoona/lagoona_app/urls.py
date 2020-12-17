from django.urls import path
from .views import LagoonaView

urlpatterns = [
    path('', LagoonaView.as_view(), name='index')
]
