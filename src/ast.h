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

class CallExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		CallExpressionAbstractSyntaxTree(const std::string& callee,
										 std::vector<std::unique_ptr<ExpressionAbstractSyntaxTree>> args) :
										 callee(callee), args(std::move(args)) { }
	private:
		std::string callee;
		std::vector<std::unique_ptr<ExpressionAbstractSyntaxTree>> args;
};

class PrototypeExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		PrototypeExpressionAbstractSyntaxTree(const std::string& name, std::vector<std::string> args) :
											  name(name), args(std::move(args)) { }

	private:
		std::string name;
		std::vector<std::string> args;
};

class FunctionExpressionAbstractSyntaxTree : public ExpressionAbstractSyntaxTree {
	public:
		FunctionExpressionAbstractSyntaxTree(std::unique_ptr<PrototypeExpressionAbstractSyntaxTree> prototype,
											 std::unique_ptr<ExpressionAbstractSyntaxTree> body) :
											prototype(std::move(prototype)), body(std::move(body)) { }

	private:
		std::unique_ptr<PrototypeExpressionAbstractSyntaxTree> prototype;
		std::unique_ptr<ExpressionAbstractSyntaxTree> body;
}; 
