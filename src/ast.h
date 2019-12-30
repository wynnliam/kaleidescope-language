#include <string>
#include <vector>
#include <memory>

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

class BinaryExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		BinaryExpressionAbstractSyntaxTree(const char op,
										   std::unique_ptr<ExpressionAbstractSyntaxTree> lhs,
										   std::unique_ptr<ExpressionAbstractSyntaxTree> rhs) :
										   op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) { }

	private:
		char op;
		std::unique_ptr<ExpressionAbstractSyntaxTree> lhs, rhs;
};
