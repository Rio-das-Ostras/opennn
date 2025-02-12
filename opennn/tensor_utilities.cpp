//   OpenNN: Open Neural Networks Library
//   www.opennn.net
//
//   T E N S O R   U T I L I T I E S   S O U R C E
//
//   Artificial Intelligence Techniques, SL
//   artelnics@artelnics.com

#include "tensor_utilities.h"

#define GET_VARIABLE_NAME(Variable) (#Variable)


namespace OpenNN
{

void initialize_sequential(Tensor<type, 1>& vector)
{
    for(Index i = 0; i < vector.size(); i++) vector(i) = type(i);
}


void multiply_rows(Tensor<type, 2>& matrix, const Tensor<type, 1>& vector)
{
    const Index columns_number = matrix.dimension(1);
    const Index rows_number = matrix.dimension(0);

//    #pragma omp parallel for

    for(Index i = 0; i < rows_number; i++)
    {
        for(Index j = 0; j < columns_number; j++)
        {
           matrix(i,j) *= vector(j);
        }
    }
}


void divide_columns(Tensor<type, 2>& matrix, const Tensor<type, 1>& vector)
{
    const Index columns_number = matrix.dimension(1);
    const Index rows_number = matrix.dimension(0);

//    #pragma omp parallel for

    for(Index j = 0; j < columns_number; j++)
    {
        for(Index i = 0; i < rows_number; i++)
        {
           matrix(i,j) /= vector(i) == type(0) ? type(1) : vector(i);
        }
    }
}


bool is_zero(const Tensor<type, 1>& tensor)
{
    const Index size = tensor.size();

    for(Index i = 0; i < size; i++)
    {
        if(abs(tensor[i]) > type(NUMERIC_LIMITS_MIN)) return false;
    }

    return true;
}

bool is_zero(const Tensor<type,1>& tensor,const type& limit)
{
    const Index size = tensor.size();

    for(Index i = 0; i < size; i++)
    {
        if(abs(tensor[i]) > type(limit)) return false;
    }

    return true;
}


bool is_false(const Tensor<bool, 1>& tensor)
{
    const Index size = tensor.size();

    for(Index i = 0; i < size; i++)
    {
        if(tensor(i)) return false;
    }

    return true;
}


bool is_binary(const Tensor<type, 2>& matrix)
{
    const Index size = matrix.size();

    for(Index i = 0; i < size; i++)
    {
        if(matrix(i) != type(0) && matrix(i) != type(1)) return false;
    }

    return true;
}


bool is_constant(const Tensor<type, 1>& vector)
{
    const Index size = vector.size();

    for(Index i = 0; i < size; i++)
    {
        for(Index j = 0; j < size; j++)
        {
            if((vector(i) - vector(j)) != type(0)) return false;
        }
    }

    return true;
}


bool is_equal(const Tensor<type, 2>& matrix, const type& value, const type& tolerance)
{
    const Index size = matrix.size();

    for(Index i = 0; i < size; i++)
    {
        if(abs(matrix(i) - value) > tolerance) return false;
    }

    return true;
}



bool are_equal(const Tensor<type, 1>& vector_1, const Tensor<type, 1>& vector_2, const type& tolerance)
{
    const Index size = vector_1.size();

    for(Index i = 0; i < size; i++)
    {
        if(abs(vector_1(i) - vector_2(i)) > tolerance) return false;
    }

    return true;
}


bool are_equal(const Tensor<type, 2>& matrix_1, const Tensor<type, 2>& matrix_2, const type& tolerance)
{
    const Index size = matrix_1.size();

    for(Index i = 0; i < size; i++)
    {
        if(abs(matrix_1(i) - matrix_2(i)) > tolerance) return false;
    }

    return true;
}


void save_csv(const Tensor<type,2>& data, const string& filename)
{
    ofstream file(filename);

    if(!file.is_open())
    {
      ostringstream buffer;

      buffer << "OpenNN Exception: Matrix template." << endl
             << "void save_csv(const Tensor<type,2>&, const string&) method." << endl
             << "Cannot open matrix data file: " << filename << endl;

      throw logic_error(buffer.str());
    }

    file.precision(20);

    const Index data_rows = data.dimension(0);
    const Index data_columns = data.dimension(1);

    char separator_char = ';';

    for(Index i = 0; i < data_rows; i++)
    {
       for(Index j = 0; j < data_columns; j++)
       {
           file << data(i,j);

           if(j != data_columns-1)
           {
               file << separator_char;
           }
       }
       file << endl;
    }
    file.close();
}


/// @todo It does not work well.

Tensor<Index, 1> calculate_rank_greater(const Tensor<type, 1>& vector)
{        
    const Index size = vector.size();

    Tensor<Index, 1> rank(size);
    iota(rank.data(), rank.data() + rank.size(), 0);

    sort(rank.data(),
         rank.data() + rank.size(),
         [&](Index i, Index j){return vector[i] > vector[j];});

    return rank;
}


Tensor<Index, 1> calculate_rank_less(const Tensor<type, 1>& vector)
{
    const Index size = vector.size();

    Tensor<Index, 1> rank(size);
    iota(rank.data(), rank.data() + rank.size(), 0);

    sort(rank.data(),
         rank.data() + rank.size(),
         [&](Index i, Index j){return vector[i] < vector[j];});

    return rank;
}


void scrub_missing_values(Tensor<type, 2>& matrix, const type& value)
{
    std::replace_if(matrix.data(), matrix.data()+matrix.size(), [](type x){return isnan(x);}, value);
}


Tensor<type, 2> kronecker_product(const Tensor<type, 1>& vector, const Tensor<type, 1>& other_vector)
{
    const Index size = vector.size();

    Tensor<type, 2> direct(size, size);

    #pragma omp parallel for

    for(Index i = 0; i < size; i++)
    {
        for(Index j = 0; j < size; j++)
        {
            direct(i, j) = vector(i) * other_vector(j);
        }
    }

    return direct;
}


type l1_norm(const ThreadPoolDevice* thread_pool_device, const Tensor<type, 1>& vector)
{
    Tensor<type, 0> norm;

    norm.device(*thread_pool_device) = vector.abs().sum();

    return norm(0);
}


void l1_norm_gradient(const ThreadPoolDevice* thread_pool_device, const Tensor<type, 1>& vector, Tensor<type, 1>& gradient)
{
    gradient.device(*thread_pool_device) = vector.sign();
}


void l1_norm_hessian(const ThreadPoolDevice*, const Tensor<type, 1>&, Tensor<type, 2>& hessian)
{
    hessian.setZero();
}


/// Returns the l2 norm of a vector.

type l2_norm(const ThreadPoolDevice* thread_pool_device, const Tensor<type, 1>& vector)
{
    Tensor<type, 0> norm;

    norm.device(*thread_pool_device) = vector.square().sum().sqrt();

    if(isnan(norm(0)))
    {
//        cout << "OpenNN Warning: l2 norm of vector is NaN" << endl;

//        ostringstream buffer;

//        buffer << "OpenNN Exception: l2 norm of vector is not a number" << endl;

//        throw logic_error(buffer.str());
    }

    return norm(0);
}


void l2_norm_gradient(const ThreadPoolDevice* thread_pool_device, const Tensor<type, 1>& vector, Tensor<type, 1>& gradient)
{
    const type norm = l2_norm(thread_pool_device, vector);

    if(norm < type(NUMERIC_LIMITS_MIN))
    {
        gradient.setZero();

        return;
    }

    gradient.device(*thread_pool_device) = vector/norm;
}


void l2_norm_hessian(const ThreadPoolDevice* thread_pool_device, const Tensor<type, 1>& vector, Tensor<type, 2>& hessian)
{
    const type norm = l2_norm(thread_pool_device, vector);

    if(norm < type(NUMERIC_LIMITS_MIN))
    {
        hessian.setZero();

        return;
    }

    hessian.device(*thread_pool_device) = kronecker_product(vector, vector)/(norm*norm*norm);
}


void sum_diagonal(Tensor<type, 2>& matrix, const type& value)
{
    const Index rows_number = matrix.dimension(0);

     #pragma omp parallel for
    for(Index i = 0; i < rows_number; i++)
        matrix(i,i) += value;
}

/// Uses Eigen to solve the system of equations by means of the Householder QR decomposition.

Tensor<type, 1> perform_Householder_QR_decomposition(const Tensor<type, 2>& A, const Tensor<type, 1>& b)
{
    const Index n = A.dimension(0);

    Tensor<type, 1> x(n);

    const Map<Matrix<type, Dynamic, Dynamic>> A_eigen((type*)A.data(), n, n);
    const Map<Matrix<type, Dynamic, 1>> b_eigen((type*)b.data(), n, 1);
    Map<Matrix<type, Dynamic, 1>> x_eigen((type*)x.data(), n);

    x_eigen = A_eigen.colPivHouseholderQr().solve(b_eigen);

    return x;
}


void fill_submatrix(const Tensor<type, 2>& matrix,
                    const Tensor<Index, 1>& rows_indices,
                    const Tensor<Index, 1>& columns_indices,
                    type* submatrix_pointer)
{
    const Index rows_number = rows_indices.size();
    const Index columns_number = columns_indices.size();

    const type* matrix_pointer = matrix.data();

    #pragma omp parallel for

    for(Index j = 0; j < columns_number; j++)
    {
        const type* matrix_column_pointer = matrix_pointer + matrix.dimension(0)*columns_indices[j];
        type* submatrix_column_pointer = submatrix_pointer + rows_number*j;

        const type* value_pointer = nullptr;
        const Index* rows_indices_pointer = rows_indices.data();
        for(Index i = 0; i < rows_number; i++)
        {
            value_pointer = matrix_column_pointer + *rows_indices_pointer;
            rows_indices_pointer++;
            *submatrix_column_pointer = *value_pointer;
            submatrix_column_pointer++;
        }
    }
}

Index count_NAN(const Tensor<type, 1>& x)
{
    Index NAN_number = 0;

    for(Index i = 0; i < x.size(); i++)
    {
        if(isnan(x(i))) NAN_number++;
    }

    return NAN_number;
}


void check_size(const Tensor<type, 1>& vector, const Index& size, const string& log)
{
    if(vector.size() != size)
    {
        ostringstream buffer;

        buffer << "OpenNN Exception: " << log
               << "Size of vector is " << vector.size() << ", but must be " << size << ".";

        throw logic_error(buffer.str());
    }
}


void check_dimensions(const Tensor<type, 2>& matrix, const Index& rows_number, const Index& columns_number, const string& log)
{
    if(matrix.dimension(0) != rows_number)
    {
        ostringstream buffer;

        buffer << "OpenNN Exception: " << log
               << "Number of rows in matrix is " << matrix.dimension(0) << ", but must be " << rows_number << ".";

        throw logic_error(buffer.str());
    }

    if(matrix.dimension(1) != columns_number)
    {
        ostringstream buffer;

        buffer << "OpenNN Exception: " << log
               << "Number of columns in matrix is " << matrix.dimension(0) << ", but must be " << columns_number << ".";

        throw logic_error(buffer.str());
    }
}


void check_columns_number(const Tensor<type, 2>& matrix, const Index& columns_number, const string& log)
{
    if(matrix.dimension(1) != columns_number)
    {
        ostringstream buffer;

        buffer << "OpenNN Exception: " << log
               << "Number of columns in matrix is " << matrix.dimension(0) << ", but must be " << columns_number << ".";

        throw logic_error(buffer.str());
    }
}

Tensor<type, 2> assemble_vector_vector(const Tensor<type, 1>& x, const Tensor<type, 1>& y)
{
    const Index rows_number = x.size();
    const Index columns_number = 2;

    Tensor<type, 2> data(rows_number, columns_number);

    for(Index i = 0; i < rows_number; i++)
    {
        data(i, 0) = x(i);
        data(i, 1) = y(i);
    }

    return data;
}


Tensor<type, 2> assemble_vector_matrix(const Tensor<type, 1>& x, const Tensor<type, 2>& y)
{
    const Index rows_number = x.size();
    const Index columns_number = 1 + y.dimension(1);

    Tensor<type, 2> data(rows_number, columns_number);

    for(Index i = 0; i < rows_number; i++)
    {
        data(i, 0) = x(i);

        for(Index j = 0; j < y.dimension(1); j++)
        {
            data(i, 1+j) = y(i,j);
        }
    }

    return data;
}


Tensor<type, 2> assemble_matrix_vector(const Tensor<type, 2>& x, const Tensor<type, 1>& y)
{
    const Index rows_number = x.size();
    const Index columns_number = x.dimension(1) + 1;

    Tensor<type, 2> data(rows_number, columns_number);

    for(Index i = 0; i < rows_number; i++)
    {
        for(Index j = 0; j < x.dimension(1); j++)
        {
            data(i, j) = x(i,j);
        }

        data(i, columns_number-1) = y(i);
    }

    return data;
}


Tensor<type, 2> assemble_matrix_matrix(const Tensor<type, 2>& x, const Tensor<type, 2>& y)
{
    const Index rows_number = x.dimension(0);
    const Index columns_number = x.dimension(1) + y.dimension(1);

    Tensor<type, 2> data(rows_number, columns_number);

    for(Index i = 0; i < rows_number; i++)
    {
        for(Index j = 0; j < x.dimension(1); j++)
        {
            data(i,j) = x(i,j);
        }

        for(Index j = 0; j < y.dimension(1); j++)
        {
            data(i, x.dimension(1) + j) = y(i,j);
        }
    }

    return data;
}


/// Returns true if any value is less or equal than a given value, and false otherwise.

bool is_less_than(const Tensor<type, 1>& column, const type& value)
{
    const Tensor<bool, 1> if_sentence = column <= column.constant(value);

    Tensor<bool, 1> sentence(column.size());
    sentence.setConstant(true);

    Tensor<bool, 1> else_sentence(column.size());
    else_sentence.setConstant(false);

    const Tensor<bool, 0> is_less = (if_sentence.select(sentence, else_sentence)).any();

    return is_less(0);
}


}


// OpenNN: Open Neural Networks Library.
// Copyright(C) 2005-2021 Artificial Intelligence Techniques, SL.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
