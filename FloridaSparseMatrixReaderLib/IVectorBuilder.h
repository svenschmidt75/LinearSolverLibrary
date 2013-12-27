/*
 * Name  : IVectorBuilder
 * Path  : 
 * Use   : Base class for a vector builder object
 * Author: Sven Schmidt
 * Date  : 08/12/2012
 */
#pragma once


namespace EntityReader_NS {

    template<typename T>
    class IVectorBuilder {
    public:
        typedef std::shared_ptr<T> result_t;

    public:
        virtual ~IVectorBuilder() {}

        virtual void     init(LinAlg_NS::Vector::size_type dim) const = 0;
        virtual void     insertVectorElement(LinAlg_NS::Vector::size_type row, double value) const = 0;
        virtual result_t result() const = 0;
    };

} // namespace EntityReader_NS
