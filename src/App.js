import './App.css';
import React, { Component } from 'react';
import Company from './Company';
import Employee from './Employee';

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      companyName: '',
      companyId: '',
      employeeName: '',
      position: '',
      company_id: 0,
      employee_id: 0,
      companies: [],
      employees: [],
    };
  }

  inputHandler = (e) => {
    const { name, value } = e.target;
    // eslint-disable-next-line
    if (name == "companyId") {
      const company_ids = this.state.companies.map((item) => item.company_id)
      if (company_ids.includes(parseInt(value))) {
        console.log("sucess")
      } else {
        alert("Company does not exist!");
        Array.from(document.querySelectorAll('input')).forEach(
          input => (input.value = '')
        );
        this.setState({companyId: ''})
      }
    }
    this.setState({ [name]: value });
  }
  
  addInputCompany = () => {
    const company_names = this.state.companies.map((item) => item.company_name)
    if (company_names.includes(this.state.companyName)) {
      alert("Company already exist!");
      Array.from(document.querySelectorAll('input')).forEach(
        input => (input.value = '')
      );
      this.setState({companyName: ''})
    } else {
      this.setState({ companies: [...this.state.companies, { company_id: this.state.company_id + 1, company_name: this.state.companyName }] })
      this.setState({ company_id: this.state.company_id + 1 })
      Array.from(document.querySelectorAll('input')).forEach(
        input => (input.value = '')
      );
      this.setState({companyName: ''})
    }
  }

  addInputEmployee = () => {
    this.setState({ employees: [...this.state.employees, { employee_id: this.state.employee_id + 1, company_id: this.state.companyId, employee_name: this.state.employeeName, employee_position: this.state.position }] })
    this.setState({ employee_id: this.state.employee_id + 1 })
    Array.from(document.querySelectorAll('input')).forEach(
      input => (input.value = '')
    );
    this.setState({companyId: ''})
    this.setState({employeeName: ''})
    this.setState({position: ''})
  }

  render() {
    return (
      <div className="">
        <div className="activity">
          <h1 className="header">Companies</h1>
          <Company companies={this.state.companies} companyName={this.state.companyName} inputHandler={this.inputHandler} addInputCompany={this.addInputCompany}/>
        </div>
        <div className="employees">
          <h1 className="header">Employees</h1>
          <Employee employees={this.state.employees} companyId={this.state.companyId} employeeName={this.state.employeeName} position={this.state.position} inputHandler={this.inputHandler} addInputEmployee={this.addInputEmployee}/>
        </div>
      </div>
    );
  }
}
export default App;
