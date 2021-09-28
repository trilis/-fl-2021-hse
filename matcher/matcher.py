class Type:
	empty = 0
	eps = 1
	star = 2
	orr = 3
	concatenation = 4

class Matcher:
	def copyassign(self, expr):
		pass

	@staticmethod
	def nullable(expr):
		pass

	def __init__(self, type = None, expr1 = None, expr2 = None):
		if type == None:
			raise 'No type passed'
		if expr1 != None and expr2 != None:
			self.type = type
			if type == Type.orr:
				if expr1.type == Type.empty:
					self.copyassign(expr2)
				elif expr2.type == Type.empty:
					self.copyassign(expr1)
				elif expr1.type == Type.eps:
					if Matcher.nullable(expr2):
						self.copyassign(expr2)
				elif expr2.type == Type.eps:
					if Matcher.nullable(expr1):
						self.copyassign(expr1)
				else:
					self.or_slots = [expr1, expr2]
			if type == Type.concatenation:
				if expr1.type == Type.empty or expr2.type == Type.empty:
					self.type = Type.empty
				elif expr1.type == Type.eps:
					self.copyassign(expr2)
				elif expr2.type == Type.eps:
					self.copyassign(expr1)
				else:
					self.or_slots = [expr1, expr2]
		elif expr1 != None:
			if expr1.type == Type.eps:
				self.copyassign(Matcher(Type.eps))
			elif expr1.type == Type.empty:
				self.copyassign(Matcher(Type.empty))
			elif expr1.type == Type.star:
				self.copyassign(expr1)
