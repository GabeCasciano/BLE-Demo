from PyQt5.QtCore import pyqtSlot
from PyQt5.QtWidgets import QHBoxLayout, QLabel, QSizePolicy, QVBoxLayout, QWidget
import pyqtgraph as pg
import numpy as np
from pydantic import BaseModel


class ConfigModel(BaseModel):
    class LabelModel(BaseModel):
        text: str = "label"
        size: int = 20
    buffer: int = 100
    title: LabelModel
    y_label: LabelModel
    x_label: LabelModel
    data_label: str = "data"
    color: str = "#ff0000"

class PlotWidget(QWidget):
    class Plot(pg.PlotWidget):
        def __init__(self, config: ConfigModel, width: int = 200, height: int = 100, parent = None):
            super().__init__(parent)

            self.config = config
            self.plotItem.setLabel(
                "left",
                f'<span style="font-size: {str(self.config.y_label.size)}px">{self.config.y_label.text}</span>',
            )
            self.plotItem.setLabel(
                "bottom",
                f'<span style="font-size: {str(self.config.x_label.size)}px">{self.config.x_label.text}</span>',
            )

            self.plotItem.setTitle(
                f'<span style="font-size: {str(self.config.title.size)}px">{self.config.title.text}</span>'
            )

            self.plotItem.showGrid(True, True, 0.5)
            self.plotItem.enableAutoRange(True, True)

            self._plot = self.plotItem.plot(pen=pg.mkPen(color=self.config.color, width=2))

            self._plot.setClipToView(True)

            self._np_arr = np.zeros(self.config.buffer, dtype=np.float32)

            self.setMinimumSize(width, height)
            self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        @pyqtSlot()
        def ClearPlot(self):
            self._np_arr = np.zeros(self.config.buffer, dtype=np.float32)

        @pyqtSlot(float)
        def Update(self, data: float):
            self._np_arr = np.roll(self._np_arr, -1)
            self._np_arr[-1] = data
            self._plot.setData(y=self._np_arr)


    def __init__(self, config: ConfigModel, width: int = 200, height: int = 100, parent = None):
        super().__init__(parent)
        self.config = config
        self._plot = PlotWidget.Plot(config, width, height, parent)

        v_box = QVBoxLayout()
        self.data_label = QLabel(f"{self.config.data_label}: 0")
        self.data_label.setStyleSheet(f"font-size: 20px; color: {self.config.color}")

        v_box.addWidget(self._plot)
        v_box.addWidget(self.data_label)

        self.setLayout(v_box)

    def _set_data_label(self, data:float):
        self.data_label.setText(f"{self.config.data_label}: {data}")

    @pyqtSlot()
    def ClearPlot(self):
        self._plot.ClearPlot()
        self._set_data_label(0)

    @pyqtSlot(float)
    def Update(self, data:float)
        self._plot.Update(data)
        self._set_data_label(data)

