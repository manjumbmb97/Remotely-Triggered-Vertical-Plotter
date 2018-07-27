from flask import Flask
from config import Config
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate

plotterapp = Flask(__name__)
plotterapp.config.from_object(Config)
db = SQLAlchemy(plotterapp)
migrate = Migrate(plotterapp, db)

from plotterapp import routes, models