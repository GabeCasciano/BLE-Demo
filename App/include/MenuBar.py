import logging
from PyQt5.QtCore import pyqtSlot
from PyQt5.QtWidgets import QAction, QMenuBar, QInputDialog
from .Mqtt import MqttClient


class MenuBar(QMenuBar):

    CloseAppAction = QAction("Close App")
    ConnectMqttAction = QAction("Conect Mqtt")
    DisconnectMqttAction = QAction("Disconnect Mqtt")

    SetLEDAction = QAction("Set LED Value")

    _instance = None

    LOG_FMT_STR = f"[Menubar] - %s"

    @classmethod
    def get_instance(cls, parent=None):
        if cls._instance is None:
            cls._instance = cls(parent)
        return cls._instance

    def __init__(self, parent=None):
        super().__init__(parent)

        self._m_client = MqttClient.get_instance()

        app_menu = self.addMenu("App")
        command_menu = self.addMenu("Commands")
     
        mqtt_submenu = app_menu.addMenu("Mqtt")
        app_menu.addAction(MenuBar.CloseAppAction)
        mqtt_submenu.addAction(MenuBar.ConnectMqttAction)
        mqtt_submenu.addAction(MenuBar.DisconnectMqttAction)

        command_menu.addAction(MenuBar.SetLEDAction)
      
        if parent is not None:
            self.CloseAppAction.triggered.connect(parent.close)

        MenuBar.ConnectMqttAction.triggered.connect(self._m_client.ConnectBroker)
        MenuBar.DisconnectMqttAction.triggered.connect(self._m_client.DisconnectBroker)
        MenuBar.SetLEDAction.triggered.connect(self.SetLEDDialog)

    def SetLEDDialog(self):
        value, ok = QInputDialog.getInt(self, "Set LED Value", "Value:", step=1, min=0, max=255)

        if ok and value >= 0:
            self._m_client.SetLEDValue(value)
        else: 
            logging.warning(MenuBar.LOG_FMT_STR, "User cancelled or Value was None")
   
