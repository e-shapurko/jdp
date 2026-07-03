#ifndef IJBASIC_H
#define IJBASIC_H   

#include "ijnode.h"

class IJBasic: public IJNode
{
public:
	IJBasic(const std::string &key):IJNode(key){}
    virtual ~IJBasic() = default;

    virtual int getInt() const { throw std::runtime_error("This is not Int type"); }
    virtual float getFloat() const { throw std::runtime_error("This is not Float type"); }
    virtual double getDouble() const { throw std::runtime_error("This is not Double type"); }
    virtual std::string getString() const { throw std::runtime_error("This is not String type"); }
    virtual bool getBool() const { throw std::runtime_error("This is not Bool type"); }

	virtual void setInt(int) { throw std::runtime_error("Can not set Int value"); }
	virtual void setFloat(float) { throw std::runtime_error("Can not set Float value"); }
    virtual void setDouble(double) { throw std::runtime_error("Can not set Double value"); }
	virtual void setString(const std::string&) { throw std::runtime_error("Can not set String value"); }
	virtual void setBool(bool) { throw std::runtime_error("Can not set Bool value"); }

    virtual Json saveAsJson() const override = 0;
    virtual IJNode::TypeIndex typeIndex() const override = 0;	
};







#endif

