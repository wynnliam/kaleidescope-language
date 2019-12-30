#include <string>
#include <vector>

class ExpressionAbstractSyntaxTree {
	public:
		virtual ~ExpressionAbstractSyntaxTree();
};

class NumberExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		NumberExpressionAbstractSyntaxTree(const double val) : value(val) { }

	private:
		double value;
};

class VariableExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		VariableExpressionAbstractSyntaxTree(const std::string& name) : name(name) { }
	
	private:
		std::string name;
};
