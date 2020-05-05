# Sequential_File_Structure_Simulator 
<strong>(순차 파일 구조 시뮬레이터)</strong>

- A sequential file is the most basic method of organizing records. It is a method of sequentially storing records in sequence when creating a file.

- In this type of sequential file structure, if you want to Add new record, Delete existing record, or Correct existing data in the master file, you must use batch processing.

- Batch processing creates a transaction file of what you want to do with an existing master file (add a new record, delete an existing record, modify an existing record), and processes the existing master file and transaction file according to the key value (data value of a record that can be sorted) and the action you want to perform (Insert, Delete, Correct) when you renew the new master file.

- This Sequential File Structure Simulator is a program that simulates key sequential files and functions are shown in the following table

<br></br>
<strong>< Sequential File Structure Simulator Functions List ></strong>

| Index | Functions |
|:---:|:---|
| (1) | Error detection and output of input data through editing |
| (2) | Outputs student information master file (before and after renewal) |
| (3) | Update student information master file via transaction file (add, delete, modify) |
| (4) | Configuring and printing transaction files for updates |
| (5) | Output error logs during editing and transaction error logs during master file renewal |

<br></br>

<strong>< Algorithm for update Master File ></strong>

| Case | Action |
|:---:|:---|
| masterKey < transKey | If the key value in the transaction file is bigger than the master key value, then there is no record to apply to the existing master file, so copy the existing master record to the new master file and read the next master record |
| masterKey == transKey | (1) Insert Operation : Duplicate record error, read next transaction record because the master file already has a record with the same key value <br> (2) Delete Operation : Ignore existing master records, read next master and transaction records without processing <br> (3) Correct(Modify) Operation : Writes the contents of an existing master record to a new master file as the contents of a transaction record, and reads the next master and transaction record |
| masterKey > transKey | (1) Insert Operation : Insert transaction record into new master file, read next transaction record <br> (2) Delete Operation : Error because no matching master record exists, read next transaction record <br> (3) Correct(Modify) Operation : Error because no matching master record exists, read next transaction record |

 <strong>
- masterKey : Key values for the master record that can be sorted<br>
- transKey : Key values for the transaction record that can be sorted
</strong>
<br></br>
[![HitCount](http://hits.dwyl.com/hyung8789/Sequential_File_Structure_Simulator.svg)](http://hits.dwyl.com/hyung8789/Sequential_File_Structure_Simulator)
