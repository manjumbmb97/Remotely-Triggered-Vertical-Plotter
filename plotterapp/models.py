from datetime import datetime
from plotterapp import db

class Canvas(db.Model):
	id = db.Column(db.Integer, primary_key=True)
	project_name = db.Column(db.String(500), index=True, unique=False)
	scale_type = db.Column(db.String(20))
	width = db.Column(db.Float)
	height = db.Column(db.Float)
	mounting_distance = db.Column(db.Float)
	origin_distance = db.Column(db.Float)

	def __repr__(self):
		return '<Canvas {}>'.format(self.project_name)