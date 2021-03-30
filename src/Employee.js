import './App.css';

function Employees(props) {
    const { employees, companyId, employeeName, position, inputHandler, addInputEmployee } = props;
    return (
        <div className="employee">
            <input type="text" name="companyId" onChange={inputHandler} placeholder="Enter company id" required></input>
            <input type="text" name="employeeName" onChange={inputHandler} placeholder="Enter an employee name" required></input>
            <input type="text" name="position" onChange={inputHandler} placeholder="Enter employee position" required></input>
            <button disabled={!companyId || !employeeName || !position} onClick={addInputEmployee}>Add</button>
            <br/><br/>
            <table>
                <thead>
                    <tr>
                        <th>Employee Id</th>
                        <th>Company Id</th>
                        <th>Employee Name</th>
                        <th>Position</th>
                    </tr>
                </thead>
                <tbody>
                    {employees.map((item, index) => (
                        <tr key={index}>
                            <td>{item.employee_id}</td>
                            <td>{item.company_id}</td>
                            <td>{item.employee_name}</td>
                            <td>{item.employee_position}</td>
                        </tr>
                    ))}
                </tbody>
            </table>
        </div>
    );
}

export default Employees;